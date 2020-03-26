#include "logger.h"


namespace LoggerSpace
{ 
   
    Logger_ver_0::Logger_ver_0()
    {
        int i = 0;
        HANDLE init_count_file;
        std::string namefile;
        GetLocalTime(&t_last);
        NameFile = take_log_name(namelog);
        do
        {
            namefile = take_log_name(namelog ,count_file);
            init_count_file = CreateFileA(namefile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
            if (init_count_file != INVALID_HANDLE_VALUE) count_file++;
            CloseHandle(init_count_file);
        } while (init_count_file != INVALID_HANDLE_VALUE);
        
        work_river = 1;
        river_write = std::thread(&Logger_ver_0::tread_write_log,this);
    }

    Logger_ver_0::Logger_ver_0(const char* str)
    {
        namelog.clear();
        namelog = str;
        int i = 0;
        HANDLE init_count_file = INVALID_HANDLE_VALUE;
        std::string namefile;
        GetLocalTime(&t_last);
        NameFile = take_log_name(namelog);
        do
        {
            namefile = take_log_name(namelog, count_file);
            init_count_file = CreateFileA(namefile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
            if (init_count_file != INVALID_HANDLE_VALUE) count_file++;
            CloseHandle(init_count_file);
        } while (init_count_file != INVALID_HANDLE_VALUE);

        work_river = 1;
        river_write = std::thread(&Logger_ver_0::tread_write_log, this);
    }

    void Logger_ver_0::WriteLogMesseng(LoggerSpace::log_mode current_mode,const char** form)
    {
        SYSTEMTIME st;
        std::string helpstr;
        std::string time;
        char c[25];
        try
        {
      
            GetLocalTime(&st);
            sprintf(c, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
            time = c;
            helpstr = time;
            switch (current_mode)
            {
            case DEBUG:
                helpstr += "DEBUG\t";
                break;
            case INFO:
                helpstr += "INFO\t";
                break;
            case WARNING:
                helpstr += "WARNING\t";
                break;
            case ERR:
                helpstr += "ERROR\t";
                break;
            default:
                break;
            }
            helpstr += *form;
            helpstr += '\n';
        }
        catch (...) {}

        event_protect_WrDel_list.lock();
        try { LogList[work_list].push_back(helpstr); }
        catch(...){}
        event_protect_WrDel_list.unlock();

        if (event_write.try_lock() == true)
        {
            if (work_river == 0)
            {
                work_river = 1;
                river_write = std::thread(&Logger_ver_0::tread_write_log, this);
            }
            event_write.unlock();
        }
    }

    void Logger_ver_0::WriteLogDEBUG(const char* form)
    {
        if (mode == DEBUG) Logger_ver_0::WriteLogMesseng(DEBUG,&form);
    }

    void Logger_ver_0::WriteLogINFO(const char* form)
    {     
        if (mode <= INFO) Logger_ver_0::WriteLogMesseng(INFO, &form);
    }
    void Logger_ver_0::WriteLogWARNING(const char* form)
    {

        if (mode <= WARNING) Logger_ver_0::WriteLogMesseng(WARNING, &form);
    }
    
    void Logger_ver_0::WriteLogERR(const char* form)
    {
        Logger_ver_0::WriteLogMesseng(ERR, &form);
    }

    int Logger_ver_0::tread_write_log()
    {
            SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
            HANDLE file_log = NULL;
            UINT64 t1, t2;
            SYSTEMTIME t_now;
            std::string str;
            LARGE_INTEGER SizeFileNow;
            char numlist = 0;
            char count_empty_list = 0;
            try 
            {
            next_iter:
                GetLocalTime(&t_now);
                SystemTimeToFileTime(&t_last, (FILETIME*)&t1);
                SystemTimeToFileTime(&t_now, (FILETIME*)&t2);
                t1 = t1 / 1000 / 1000 / 10 / 60 / 60 / 24;
                t2 = t2 / 1000 / 1000 / 10 / 60 / 60 / 24;
                if ((t2 - t1) >= DayWrite)
                {
                    str.clear();
                    str = NameFile;
                    str.insert(NameFile.find('.', 0), "_");
                    str.insert(str.find('.', 0), std::to_string(count_file));
                    MoveFileA(NameFile.c_str(), str.c_str());
                    GetLocalTime(&t_last);
                    NameFile = take_log_name(namelog);
                    count_file = 0;
                }

                event_protect_WrDel_list.lock();
                numlist = work_list;
                work_list = (work_list + 1) & 1;
                event_protect_WrDel_list.unlock();

                if (!LogList[numlist].empty())
                {
                    count_empty_list = 0;
                    file_log = CreateFileA(NameFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
                    SetFilePointer(file_log, 0, 0, FILE_END);
                    while (!LogList[numlist].empty())
                    {
                        str.clear();
                        str = LogList[numlist].front();
                        WriteFile(file_log, str.c_str(), str.length(), NULL, NULL);
                        LogList[numlist].pop_front();
                    }
                    GetFileSizeEx(file_log, &SizeFileNow);
                    if (SizeFileNow.QuadPart > SizeLogFile)
                    {
                        CloseHandle(file_log);
                        str.clear();
                        str = NameFile;
                        str.insert(NameFile.find('.', 0), "_");
                        str.insert(str.find('.', 0), std::to_string(count_file));
                        count_file++;
                        MoveFileA(NameFile.c_str(), str.c_str());
                        if (NameFile != take_log_name(namelog))
                        {
                            NameFile = take_log_name(namelog);
                            count_file = 0;
                        }
                        GetLocalTime(&t_last);
                    }
                    else CloseHandle(file_log);
                }
                else
                {
                    count_empty_list++;
                    if (count_empty_list >= 2)
                    {
                        count_empty_list = 0;
                        if (flag_end == 1) return 0;
                        Sleep(1);
                    }
                }
                if (flag_end == 1) return 0;
                goto next_iter;
            }
            catch (...)
            {
                work_river = 0;
            }           
            return 0;
    }

    Logger_ver_0::~Logger_ver_0()
    {
        flag_end = 1;
        river_write.join();
    }

    std::string take_log_name(std::string first_str,int count)
    {
        std::string logname;
        SYSTEMTIME t;
        GetLocalTime(&t);
        logname = first_str+'_';
        logname += std::to_string(t.wYear)+'-';
        if (t.wMonth < 10) { logname += '0' + std::to_string(t.wMonth) + '-'; }
        else { logname += std::to_string(t.wMonth) + '-'; }
        if (t.wDay < 10) { logname += '0' + std::to_string(t.wDay); }
        else { logname += std::to_string(t.wDay);} 
        
        if (count != -1) logname += '_' + std::to_string(count);
        logname += ".txt";

        return logname;
    }

    size_t Logger_ver_0::SizeLogList()
    {
        return LogList[work_list].size();
    }

    void Logger_ver_0::SetNameLog(const char* str)
    {
        HANDLE init_count_file;        
        namelog.clear();
        namelog = str;
        std::string namefile;
        count_file = 0;
        GetLocalTime(&t_last);
        NameFile = take_log_name(namelog);
        do
        {
            namefile = take_log_name(namelog, count_file);
            init_count_file = CreateFileA(namefile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
            if (init_count_file != INVALID_HANDLE_VALUE) count_file++;
            CloseHandle(init_count_file);
        } while (init_count_file != INVALID_HANDLE_VALUE);
    }
}