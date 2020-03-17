#include "logger.h"

namespace LoggerSpace
{
    DWORD WINAPI draft_stream(LPVOID input)
    {
        Logger_ver_0* param = (Logger_ver_0*)input;
        return param->tread_write_log();
    }

    Logger_ver_0::Logger_ver_0()
    {
        int i = 0;
        HANDLE init_count_file = INVALID_HANDLE_VALUE;
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

        do
        {
            NameEvent = "logevent";
            NameEvent += std::to_string(i);
            event_write = CreateEvent(NULL, FALSE, TRUE, (LPCWSTR)&NameEvent);
            i++;
        } while (event_write == NULL || GetLastError() == ERROR_ALREADY_EXISTS);

        do
        {
            NameEvent_list = "WrDel_list";
            NameEvent_list += std::to_string(i);
            event_protect_WrDel_list = CreateEvent(NULL, FALSE, TRUE, (LPCWSTR)&NameEvent_list);
            i++;
        } while (event_protect_WrDel_list == NULL || GetLastError() == ERROR_ALREADY_EXISTS);
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

        do
        {
            NameEvent = "logevent";
            NameEvent += std::to_string(i);
            event_write = CreateEvent(NULL, FALSE, TRUE, (LPCWSTR)&NameEvent);
            i++;
        } while (event_write == NULL || GetLastError() == ERROR_ALREADY_EXISTS);

        do
        {
            NameEvent_list = "WrDel_list";
            NameEvent_list += std::to_string(i);
            event_protect_WrDel_list = CreateEvent(NULL, FALSE, TRUE, (LPCWSTR)&NameEvent_list);
            i++;
        } while (event_protect_WrDel_list == NULL || GetLastError() == ERROR_ALREADY_EXISTS);
    }

    void Logger_ver_0::WriteLogDEBUG(const char* form, ...)
    {

        if (mode != DEBUG) return;
        DWORD status_event;
        SYSTEMTIME st;
        std::string helpstr;
        std::string time;
        char c[25];
        GetLocalTime(&st);
        sprintf(c, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        time = c;
        helpstr = time;
        helpstr += "DEBUG\t";
        //////////////////////////////////////
        std::string str;
        char** arg = (char**)(&form);
        arg++;
        const char* formats = form;
        while (formats < (form + strlen(form))) {
            while (*formats != '%' && *formats != '\0') str += *(formats++);
            ++formats;
            switch (*formats) {
            case 'd':
                str += std::to_string((int)*((int*)(arg)));
                formats++;
                arg++;
                break;
            case 'f':
                str += std::to_string((double)*((double*)(arg)));
                formats++;
                arg++;
                break;
            }
        }
        //////////////////////////////////////////////////
        helpstr += str + '\n';
        status_event = WaitForSingleObject(event_protect_WrDel_list, 5);
        if (status_event == WAIT_OBJECT_0)
        {
            LogList.push_back(helpstr);
            SetEvent(event_protect_WrDel_list);

            status_event = WaitForSingleObject(event_write, 0);
            if (status_event == WAIT_OBJECT_0)
            {
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)draft_stream, this, NULL, NULL);
            }
        }
    }

    void Logger_ver_0::WriteLogINFO(const char* form, ...)
    {

        if (mode > INFO) return;
        DWORD status_event;
        SYSTEMTIME st;
        std::string helpstr;
        std::string time;
        char c[25];
        GetLocalTime(&st);
        sprintf(c, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        time = c;
        helpstr = time;
        helpstr += "INFO\t";
        //////////////////////////////////////
        std::string str;
        char** arg = (char**)(&form);
        arg++;
        const char* formats = form;
        while (formats < (form + strlen(form))) {
            while (*formats != '%' && *formats != '\0') str += *(formats++);
            ++formats;
            switch (*formats) {
            case 'd':
                str += std::to_string((int)*((int*)(arg)));
                formats++;
                arg++;
                break;
            case 'f':
                str += std::to_string((double)*((double*)(arg)));
                formats++;
                arg++;
                break;
            }
        }
        //////////////////////////////////////////////////
        helpstr += str + '\n';
        status_event = WaitForSingleObject(event_protect_WrDel_list, 5);
        if (status_event == WAIT_OBJECT_0)
        {
            LogList.push_back(helpstr);
            SetEvent(event_protect_WrDel_list);

            status_event = WaitForSingleObject(event_write, 0);
            if (status_event == WAIT_OBJECT_0)
            {
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)draft_stream, this, NULL, NULL);
            }
        }
    }

    void Logger_ver_0::WriteLogWARNING(const char* form, ...)
    {

        if (mode > WARNING) return;
        DWORD status_event;
        SYSTEMTIME st;
        std::string helpstr;
        std::string time;
        char c[25];
        GetLocalTime(&st);
        sprintf(c, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        time = c;
        helpstr = time;
        helpstr += "WARNING\t";
        //////////////////////////////////////
        std::string str;
        char** arg = (char**)(&form);
        arg++;
        const char* formats = form;
        while (formats < (form + strlen(form))) {
            while (*formats != '%' && *formats != '\0') str += *(formats++);
            ++formats;
            switch (*formats) {
            case 'd':
                str += std::to_string((int)*((int*)(arg)));
                formats++;
                arg++;
                break;
            case 'f':
                str += std::to_string((double)*((double*)(arg)));
                formats++;
                arg++;
                break;
            }
        }
        //////////////////////////////////////////////////
        helpstr += str + '\n';
        status_event = WaitForSingleObject(event_protect_WrDel_list, 5);
        if (status_event == WAIT_OBJECT_0)
        {
            LogList.push_back(helpstr);
            SetEvent(event_protect_WrDel_list);

            status_event = WaitForSingleObject(event_write, 0);
            if (status_event == WAIT_OBJECT_0)
            {
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)draft_stream, this, NULL, NULL);
            }
        }
    }

    void Logger_ver_0::WriteLogERR(const char* form, ...)
    {
        //if (mode > ERR) return;
        DWORD status_event;
        SYSTEMTIME st;
        std::string helpstr;
        std::string time;
        char c[25];
        GetLocalTime(&st);
        sprintf(c, "%04d/%02d/%02d\t%02d:%02d:%02d.%03d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        time = c;
        helpstr = time;
        helpstr += "ERROR\t";
        //////////////////////////////////////
        std::string str;
        char** arg = (char**)(&form);
        arg++;
        const char* formats = form;
        while (formats < (form + strlen(form))) {
            while (*formats != '%' && *formats != '\0') str += *(formats++);
            ++formats;
            switch (*formats) {
            case 'd':
                str += std::to_string((int)*((int*)(arg)));
                formats++;
                arg++;
                break;
            case 'f':
                str += std::to_string((double)*((double*)(arg)));
                formats++;
                arg++;
                break;
            }
        }
        //////////////////////////////////////////////////
        helpstr += str + '\n';
        status_event = WaitForSingleObject(event_protect_WrDel_list, 5);
        if (status_event == WAIT_OBJECT_0)
        {
            LogList.push_back(helpstr);
            SetEvent(event_protect_WrDel_list);

            status_event = WaitForSingleObject(event_write, 0);
            if (status_event == WAIT_OBJECT_0)
            {
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)draft_stream, this, NULL, NULL);
            }
        }
    }

    DWORD WINAPI Logger_ver_0::tread_write_log()
    {

        HANDLE file_log = NULL;
        UINT64 t1, t2;
        SYSTEMTIME t_now;
        std::string str;
        LARGE_INTEGER SizeFileNow;
        GetLocalTime(&t_now);
        SystemTimeToFileTime(&t_last, (FILETIME*)&t1);
        SystemTimeToFileTime(&t_now, (FILETIME*)&t2);
        t1 = t1 / 1000 / 1000 / 10 / 60 / 60 / 24;
        t2 = t2 / 1000 / 1000 / 10 / 60 / 60 / 24;
        if ((t2 - t1) > DayWrite)
        {
            GetLocalTime(&t_last);
            NameFile = take_log_name(namelog);
            count_file = 0;
        }

        file_log = CreateFileA(NameFile.c_str(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        //if (file_log != INVALID_HANDLE_VALUE && GetLastError() != 183) count_file = 0;
        SetFilePointer(file_log, 0, 0, FILE_END);

        while (!LogList.empty())
        {
            str.clear();
            try { str = LogList.front(); }
            catch (...)
            {
                LogList.clear();
                continue;
            }
            WriteFile(file_log, str.c_str(), str.length(), NULL, NULL);
            WaitForSingleObject(event_protect_WrDel_list, INFINITE);
            LogList.pop_front();
            SetEvent(event_protect_WrDel_list);
        }
        GetFileSizeEx(file_log, &SizeFileNow);
        if (SizeFileNow.QuadPart > SizeLogFile)
        {
            CloseHandle(file_log);
            str.clear();
            str = take_log_name(namelog ,count_file);
            count_file++;
            MoveFileA(NameFile.c_str(), str.c_str());
            NameFile = take_log_name(namelog);
            GetLocalTime(&t_last);
        }
        else CloseHandle(file_log);
        SetEvent(event_write);
        return 0;
    }

    Logger_ver_0::~Logger_ver_0()
    {
        WaitForSingleObject(event_write, INFINITE);
        return;
    }

    std::string take_log_name(std::string first_str,int count)
    {
        std::string logname;
        SYSTEMTIME t;
        GetLocalTime(&t);
        logname = first_str+'_';

        if (t.wDay < 10) { logname += '0' + std::to_string(t.wDay) + '-'; }
        else { logname += std::to_string(t.wDay) + '-'; }
        if (t.wMonth < 10) { logname += '0' + std::to_string(t.wMonth) + '-'; }
        else { logname += std::to_string(t.wMonth) + '-'; }
        logname += std::to_string(t.wYear);
        if (count != -1) logname += '_' + std::to_string(count);
        logname += ".txt";

        return logname;
    }

    size_t Logger_ver_0::SizeLogList()
    {
        return LogList.size();
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