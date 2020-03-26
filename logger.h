
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <list>
#include <windows.h>
#include <string> 
#pragma warning(disable : 4996)
#include <exception>
#include <thread>
#include <mutex>

namespace LoggerSpace
{
    enum log_mode
    {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERR = 3
    };

    std::string take_log_name(std::string first_str,int count = -1);

    class Logger_ver_0
    {
        char work_river = 0;
        char flag_end = 0;;
        SYSTEMTIME t_last;
        int count_file = 0;
        char work_list = 0;
        std::thread river_write;
        std::mutex event_write;
        std::mutex event_protect_WrDel_list;
        std::list <std::string> LogList[2];
        std::string NameFile;
        std::string NameEvent = "logevent";
        std::string NameEvent_list = "WrDel_list";
        std::string namelog="log";
        int tread_write_log();
        void WriteLogMesseng(LoggerSpace::log_mode current_mode,const char** form);
        
    public:

        int SizeLogFile = 1 * 1024 * 1024;
        log_mode mode = DEBUG;
        int DayWrite = 1;
        
        Logger_ver_0();
        Logger_ver_0(const char* str);
        size_t SizeLogList();
        void SetNameLog(const char* str);        
        void WriteLogDEBUG(const char* form);
        void WriteLogINFO(const char* form);
        void WriteLogWARNING(const char* form);
        void WriteLogERR(const char* form);
        ~Logger_ver_0();
    };
}

#endif 