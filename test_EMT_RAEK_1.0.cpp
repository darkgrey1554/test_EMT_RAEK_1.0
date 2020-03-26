#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <list>
#include<TCHAR.H>
#include "logger.h"


/// --- кол-во данных в общей памяти --- ///
#define T_FORWARD_DEFAULT 90
#define TIME_DIV 25

char flag_end = 0;
/// ---  общая память --- ///
HANDLE sharmemory_emt_discrete_in;
HANDLE sharmemory_emt_analog_in;
HANDLE sharmemory_emt_discrete_out;
HANDLE sharmemory_emt_analog_out;
HANDLE mutex_discrete_out;
HANDLE mutex_analog_out;
HANDLE mutex_discrete_in;
HANDLE mutex_analog_in;
char* buf_discrete_out;
char* buf_analog_out;
char* buf_discrete_in;
char* buf_analog_in;

/// --- ведение лог-файлв --- ///

/// --- конфигурация --- ///
FILE* config_file;
std::list <std::string> config_info;
std::string str_info;
int res_read = 0;
char simvol=0;
int num_adapters;
// ver_2
struct config_device
{
    std::string type_device;
    int id_device = -1;
    std::string ip_address;
    int port =0;
    std::string type_data;
    int num_data =0;
    int offset =0;
    int freqency =0;
    HANDLE hand_device=NULL;
};
std::list <config_device> v_adapters;
int count_adapt = 0;
int count_server = 0;
int count_client = 0;

struct config_memory
{
    std::string name;
    int size=0;
};
config_memory Sharmem_InDiscrete;
config_memory Sharmem_InAnalog;
config_memory Sharmem_OutDiscrete;
config_memory Sharmem_OutAnalog;

/// --- логер ---///
LoggerSpace::Logger_ver_0 *logbug= new LoggerSpace::Logger_ver_0();

/// --- вспомогательные функии --- ///
int init_struct_config_device(std::string str);
int init_struct_shared_memory(std::string str);
std::string get_time_local();

/// --- потоки SERVER CLIENT --- ///
DWORD WINAPI thread_client(LPVOID config_client);
DWORD WINAPI thread_server(LPVOID config_server);

/// --- обработчик аварийного закрытия программы ---///
LPTOP_LEVEL_EXCEPTION_FILTER old_handler = 0;
LONG WINAPI handler_crash(PEXCEPTION_POINTERS pExceptionInfo);
BOOL WINAPI close_prog(DWORD fdwCtrlType);

int main()
{
    logbug->WriteLogINFO("MAIN\tSTART_GATE");
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetConsoleCtrlHandler(close_prog, TRUE);
    HANDLE handle_window = GetConsoleWindow();
    WSADATA wsadata;   
    DWORD code_thread;
    std::string logmess;
	/// --- считывание инф с конфиг файла --- ///
    ///  --- config_ver2 --- ///
    config_file = fopen("config_file.txt", "r");
    if (config_file == NULL)
    {       
        std::cout << "MAIN\tERROR_OPENING_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAIN\tERROR_OPENING_CONFIG_FILE");
        getch();
        return -1;
    }   
    
    while (1)
    {       
        while (simvol != '\n' && res_read!=EOF)
        {
            res_read = fscanf(config_file, "%c", &simvol);
            if ((simvol > 0x1F || simvol == '\t') && res_read != EOF ) str_info += simvol;
        }

        if (str_info == "[List]" || res_read == EOF)
        {
            if (res_read == EOF)
            {
                std::cout << "MAIT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                logbug->WriteLogERR("MAIN\tERROR_FORMATION_OF_CONFIG_FILE");
                getch();
                return -1;
            }
            break;
        }
        if (str_info.substr(0, 4) == "@EMT")
        {            
            if (init_struct_shared_memory(str_info) == -1)
            {
                getch();
                return -1;
            }
        }
        else if (str_info.substr(0, 5) == "@RAEK")
        {
            if (init_struct_config_device(str_info) == -1)
            {
                getch();
                return -1;
            }
        }
        else if (str_info.substr(0, 5) != "[EMT]" && str_info.substr(0, 6) != "[RAEK]")
        {
            std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
            logbug->WriteLogERR("MAIN\tERROR_FORMATION_OF_CONFIG_FILE");
            getch();
            return -1;
        }
        str_info.clear();
        simvol = 0;
    }
    fclose(config_file);

    for (auto iter = v_adapters.begin(); iter != v_adapters.end(); iter++)
    {
        if (iter->type_device == "Server" && iter->type_data == "analog" && (iter->offset + iter->num_data) <= Sharmem_OutAnalog.size) continue;
        if (iter->type_device == "Server" && iter->type_data == "discrete" && (iter->offset + iter->num_data) <= Sharmem_OutDiscrete.size) continue;
        if (iter->type_device == "Client" && iter->type_data == "analog" && (iter->offset + iter->num_data) <= Sharmem_InAnalog.size) continue;
        if (iter->type_device == "Client" && iter->type_data == "discrete" && (iter->offset + iter->num_data) <= Sharmem_InDiscrete.size) continue;
        std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAIN\tERROR_FORMATION_OF_CONFIG_FILE");
        getch();
        return -1;        
    }    

    for (auto iter1 = v_adapters.begin(); iter1 != v_adapters.end();iter1++)
    {
        for (auto iter2 = iter1;;)
        {
            iter2++;
            if (iter2 == v_adapters.end()) break;
            if (!((iter1->offset + iter1->num_data - 1) < iter2->offset || (iter2->offset + iter2->num_data - 1) < iter1->offset) && (iter1->type_device == iter2->type_device) && (iter1->type_data == iter2->type_data))
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
                logbug->WriteLogERR("MAIN\tERROR_FORMATION_OF_CONFIG_FILE");
                getch();
                return -1;
            }
        }
    }
       
	/// --- инициализация общей памяти --- /// 

    std::string mutexname = "mutex";

	mutex_discrete_out = CreateMutexA(NULL, FALSE, (mutexname+ Sharmem_OutDiscrete.name).c_str());
	mutex_analog_out = CreateMutexA(NULL, FALSE, (mutexname + Sharmem_OutAnalog.name).c_str());
	sharmemory_emt_discrete_out = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_OutDiscrete.size*4, Sharmem_OutDiscrete.name.c_str());
	sharmemory_emt_analog_out = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_OutAnalog.size * 4, Sharmem_OutAnalog.name.c_str());
	buf_discrete_out = (char*)MapViewOfFile(sharmemory_emt_discrete_out, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_OutDiscrete.size * 4);
	buf_analog_out = (char*)MapViewOfFile(sharmemory_emt_analog_out, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_OutAnalog.size * 4);

	mutex_discrete_in = CreateMutexA(NULL, FALSE, (mutexname + Sharmem_InDiscrete.name).c_str());
	mutex_analog_in = CreateMutexA(NULL, FALSE, (mutexname + Sharmem_InAnalog.name).c_str());
	sharmemory_emt_discrete_in = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_InDiscrete.size * 4, Sharmem_InDiscrete.name.c_str());
	sharmemory_emt_analog_in = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_InAnalog.size * 4, Sharmem_InAnalog.name.c_str());
	buf_discrete_in = (char*)MapViewOfFile(sharmemory_emt_discrete_in, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_InDiscrete.size * 4);
	buf_analog_in = (char*)MapViewOfFile(sharmemory_emt_analog_in, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_InAnalog.size * 4);

	/// --- инициализацая потоков SERVER CLIENT --- ///

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        std::cout << "MAIN\tERROR_WSAStartup CODE_ERROR: " << WSAGetLastError() << std::endl;
        logbug->WriteLogERR("MAIN\tERROR_WSAStartup");
        getch();
        return -1;
    }   
    for (std::list<config_device>::iterator iter_list = v_adapters.begin(); iter_list != v_adapters.end(); iter_list++)
    {
          std::string info_log;
          if (iter_list->type_device == "Client")
          {
              iter_list->hand_device=CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_client, &(*iter_list), NULL, NULL);
              std::cout << "MAIN\tINIT_CLIENT: ID - "<<iter_list->id_device<< "\tIP - " << iter_list->ip_address << "\tPORT - " << iter_list->port << std::endl;
              info_log += "MAIT\tINIT_CLIENT - ID: " + std::to_string(iter_list->id_device) + "\tIP: " + iter_list->ip_address + "\tPORT: " + std::to_string(iter_list->port);
              logbug->WriteLogINFO(info_log.c_str());
          }
          if (iter_list->type_device == "Server")
          {
              iter_list->hand_device =CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_server, &(*iter_list), NULL, NULL);
              std::cout << "MAIT\tINIT_SERVER: ID - " << iter_list->id_device << "\tIP - " << iter_list->ip_address << "\tPORT - " << iter_list->port << std::endl;
              info_log += "MAIT\tINIT_SERVER: ID - " + std::to_string(iter_list->id_device) + "\tIP - " + iter_list->ip_address + "\tPORT - " + std::to_string(iter_list->port);
              logbug->WriteLogINFO(info_log.c_str());
          }
    }

    while (!((GetAsyncKeyState(VK_ESCAPE) & 0x8001) > 0 && GetForegroundWindow() == handle_window))
    {
        Sleep(1000);
        for (auto iter = v_adapters.begin();iter!=v_adapters.end();iter++)
        {
           
            if (GetExitCodeThread(iter->hand_device, &code_thread))
            {
                if (code_thread != STILL_ACTIVE)
                {
                    if (iter->type_device == "Server")
                    {
                        std::cout << "MAIT\tREPEAT START SERVER ID:" << iter->id_device << std::endl;
                        logmess.clear();
                        logmess+= "MAIT\tREPEAT START SERVER ID:" + std::to_string(iter->id_device);
                        logbug->WriteLogWARNING(logmess.c_str());
                        iter->hand_device = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_server, &(*iter), NULL, NULL);
                    }
                    if (iter->type_device == "Client")
                    {
                        std::cout << "MAIT\tREPEAT START CLIENT ID:" << iter->id_device << std::endl;
                        logmess.clear();
                        logmess += "MAIT\tREPEAT START CLIENT ID:" + std::to_string(iter->id_device);
                        logbug->WriteLogWARNING(logmess.c_str());
                        iter->hand_device = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_client, &(*iter), NULL, NULL);
                    }
                }
            }
        }
    }
    end_program:
    flag_end = 1;
    Sleep(3000);
    for (auto iter = v_adapters.begin(); iter != v_adapters.end(); iter++)
    {
        TerminateThread(iter->hand_device,NULL);
    }
	return 0;
}


DWORD WINAPI thread_client(LPVOID config_client)
{
	/// --- инициализация параметров клиента --- /// 
    config_device* init_client = (config_device*)config_client;
    SOCKET sock_client;
    SOCKADDR_IN adr_server;
    LARGE_INTEGER time_last_messeng;
    LARGE_INTEGER time_river_read;
    LARGE_INTEGER freqency;
    QueryPerformanceFrequency(&freqency);

    /// --- иниц пакета запроса --- ///
    char* buf_request = new char[16];
    WSABUF wsasend;
    DWORD count_send = 0;
    DWORD flag_send = 0;
    DWORD result_wait_send;
    WSAOVERLAPPED send_overlapped;
    SecureZeroMemory((PVOID)&send_overlapped, sizeof(WSAOVERLAPPED));
    send_overlapped.hEvent = WSACreateEvent();
    wsasend.buf = buf_request;
    wsasend.len = 16;

    /// --- иниц пакета приема --- ///
    int num_data = init_client->num_data;
    char* buf_read = new char[num_data * 8+12];
    WSABUF wsarecv;
    DWORD count_recv = 0;
    DWORD flag_recv = 0;
    DWORD result_wait_recv;
    WSAOVERLAPPED recv_overlapped;
    SecureZeroMemory((PVOID)&recv_overlapped, sizeof(WSAOVERLAPPED));
    recv_overlapped.hEvent = WSACreateEvent();
    wsarecv.buf = buf_read;
    wsarecv.len = num_data*8+12;
    DWORD count_get_byte = 0;
    int num_from;
    
    double time;
    int result;
    int last_error;
    int sleep_time;
    int count_buf = 0;
    
    
    int num_data_from_server = 0;
    DWORD result_wait_mutex = 0;
    int data_int=0;
    float data_float=0.;
    char* buf_cl;
    char* buf_mem;

    std::string logmess;
 
    
    try 
    {
        for (int i = 0; i < num_data * 8; i++) buf_read[i] = 0;

        /// --- соединение с сервером --- ///
    repeat_connect:
        sock_client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (sock_client == INVALID_SOCKET)
        {
            last_error = WSAGetLastError();
            logmess.clear();
            logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_INVALID_SOCKET CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogERR(logmess.c_str());
            std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_INVALID_SOCKET CODE_ERROR: " << last_error << std::endl;
            return -1;
        }

        adr_server.sin_addr.s_addr = inet_addr(init_client->ip_address.c_str());
        adr_server.sin_port = htons(init_client->port);
        adr_server.sin_family = AF_INET;

    connect_server:
        if (connect(sock_client, (sockaddr*)&adr_server, sizeof(adr_server)) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            logmess.clear();
            logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_CONNECT_WITH_SERVER: PORT " + std::to_string(init_client->port) + " CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            std::cout << "ID_CLIENT: " + std::to_string(init_client->id_device) << "\tERROR_CONNECT_WITH_SERVER: PORT " << init_client->port << " CODE_ERROR: " << last_error << std::endl;
            Sleep(5000);
            goto connect_server;
        }
        else
        {
            logmess.clear();
            logmess += "ID_CLIENT:" + std::to_string(init_client->id_device) + "\tCONNECT_WITH_SERVER_DONE: PORT " + std::to_string(init_client->port);
            logbug->WriteLogINFO(logmess.c_str());
            std::cout << "ID_CLIENT: " << std::to_string(init_client->id_device) << "\tCONNECT_WITH_SERVER_DONE: PORT " << init_client->port << std::endl;
        }

        /// --- формирование запроса --- ///
        memset(buf_request, 0, 16);
        buf_request[0] = 2;
        QueryPerformanceCounter(&time_last_messeng);
        ///  --- отправка запроса --- ///
    next:
        if (flag_end == 1) goto end_river;
        QueryPerformanceCounter(&time_river_read);
        time = (time_river_read.QuadPart - time_last_messeng.QuadPart) * 1000.0 / freqency.QuadPart;
        if (time > init_client->freqency)
        {
            logmess.clear();
            logmess += "ID_CLIENT:" + std::to_string(init_client->id_device) + "\tLIMIT_TIME_MESSENG_READING_EXCEEDED " + " time - " + std::to_string(time) + "ms";
            logbug->WriteLogWARNING(logmess.c_str());
            std::cout << "ID_CLIENT: " << init_client->id_device << "\tLIMIT_TIME_MESSENG_READING_EXCEEDED " << time << "ms\t" << get_time_local() << std::endl;
        }
    wwait:
        QueryPerformanceCounter(&time_river_read);
        time = (time_river_read.QuadPart - time_last_messeng.QuadPart) * 1000.0 / freqency.QuadPart;
        if (time < (init_client->freqency - TIME_DIV))
        {
            Sleep(1);
            goto wwait;
        }

        /// --- отправка запроса на чтение --- ///
    repeat_send_read:
        QueryPerformanceCounter(&time_last_messeng);

        if (WSASend(sock_client, &wsasend, 1, &count_send, flag_send, &send_overlapped, NULL) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            if (last_error != WSA_IO_PENDING)
            {
                std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_SEND_MESSENG " << init_client->id_device << " CODE_ERROR: " << last_error << std::endl;
                logmess.clear();
                logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_SEND_MESSENG CODE_ERROR: " + std::to_string(last_error);
                logbug->WriteLogWARNING(logmess.c_str());
                if (last_error == 10035 || last_error == 10038 || last_error == 10050 || last_error == 10051 || last_error == 10052 ||
                    last_error == 10053 || last_error == 10054 || last_error == 10057 || last_error == 10058 || last_error == 10053 ||
                    last_error == 10061 || last_error == 10064 || last_error == 10065 || last_error == 10101)
                {
                    sleep_time = 2000;
                reconnect:
                    std::cout << "ID_CLIENT: " << init_client->id_device << "\tRECONNECT..." << std::endl;
                    logmess.clear();
                    logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tRECONNECT...";
                    logbug->WriteLogINFO(logmess.c_str());
                    closesocket(sock_client);
                    sock_client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
                    if (connect(sock_client, (sockaddr*)&adr_server, sizeof(adr_server)) == SOCKET_ERROR)
                    {
                        last_error = WSAGetLastError();
                        std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_CONNECTING_PORT: " << init_client->port << " CODE_ERROR: " << std::to_string(last_error);
                        logmess.clear();
                        logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "ERROR_CONNECTING_PORT: " + std::to_string(init_client->port) + " CODE_ERROR: " + std::to_string(last_error);
                        logbug->WriteLogWARNING(logmess.c_str());
                        Sleep(sleep_time);
                        sleep_time += sleep_time;
                        if (sleep_time >= 60000) sleep_time = 2000;
                        goto reconnect;
                    }
                    std::cout << "ID_CLIENT: " << init_client->id_device << "\tCONNECT_WITH_SERVER_DONE: PORT " << init_client->port << std::endl;
                    logmess.clear();
                    logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tCONNECT_WITH_SERVER_DONE: PORT " + std::to_string(init_client->port);
                    logbug->WriteLogINFO(logmess.c_str());
                    goto repeat_send_read;
                }
            }
        }

        result_wait_send = WSAWaitForMultipleEvents(1, &send_overlapped.hEvent, TRUE, INFINITE, TRUE);
        if (result_wait_send == WSA_WAIT_FAILED)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_WAIT_EVENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_WAIT_EVENT CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(sock_client);
            goto repeat_connect;
        }
        WSAResetEvent(send_overlapped.hEvent);

        WSAGetOverlappedResult(sock_client, &send_overlapped, &count_send, FALSE, &flag_send);
        if (count_send != 16)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_REQEST CLIENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_REQEST CLIENT CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(sock_client);
            goto repeat_connect;
        }

        /// --- прием сообщения --- /// 
        count_recv = 0;
        num_data_from_server = 0;
    next_recv:
        wsarecv.buf = buf_read + count_recv;
        wsarecv.len = num_data * 8 + 12 - count_recv;
        count_get_byte = 0;
        flag_recv = 0;

        if (WSARecv(sock_client, &wsarecv, 1, &count_get_byte, &flag_recv, &recv_overlapped, NULL) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            if (last_error != WSA_IO_PENDING)
            {
                std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_RECV CLIENT_ID: CODE_ERROR: " << last_error << std::endl;
                logmess.clear();
                logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_RECV CLIENT_ID : CODE_ERROR: " + std::to_string(last_error);
                logbug->WriteLogWARNING(logmess.c_str());
                closesocket(sock_client);
                goto repeat_connect;
            }
        }

        result_wait_recv = WSAWaitForMultipleEvents(1, &recv_overlapped.hEvent, TRUE, INFINITE, TRUE);
        if (result_wait_recv == WSA_WAIT_FAILED)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_WAIT_EVENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_WAIT_EVEТЕ CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(sock_client);
            goto repeat_connect;
        }
        WSAResetEvent(recv_overlapped.hEvent);

        if (!WSAGetOverlappedResult(sock_client, &recv_overlapped, &count_get_byte, FALSE, &flag_recv))
        {
            last_error = WSAGetLastError();
            std::cout << "ID_CLIENT: " << init_client->id_device << "\tERROR_READ_SERVER CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tERROR_READ_SERVER CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(sock_client);
            goto repeat_connect;
        }
        count_recv += count_get_byte;
        if (count_recv >= 4 && num_data_from_server == 0)
        {
            num_data_from_server = *((int*)buf_read);
            num_data_from_server = num_data_from_server * sizeof(double);
        }
        if (count_recv < num_data_from_server + 12) goto next_recv;

        if (init_client->type_data == "discrete")
        {
            result_wait_mutex = WaitForSingleObject(mutex_discrete_in, 4);
            if (result_wait_mutex == WAIT_OBJECT_0)
            {
                buf_mem = buf_discrete_in + init_client->offset * sizeof(int);
                buf_cl = buf_read + 12;
                for (int i = 0; i < num_data; i++)
                {
                    data_int = (int)(*((double*)buf_cl));
                    for (int j = 0; j < 4; j++)
                    {
                        *buf_mem = *((char*)&data_int + j);
                        buf_mem++;
                    }
                    buf_cl += 8;
                }
            }
            ReleaseMutex(mutex_discrete_in);
        }
        if (init_client->type_data == "analog")
        {
            result_wait_mutex = WaitForSingleObject(mutex_analog_in, 4);
            if (result_wait_mutex == WAIT_OBJECT_0)
            {
                buf_mem = buf_analog_in + init_client->offset * sizeof(float);
                buf_cl = buf_read + 12;
                for (int i = 0; i < num_data; i++)
                {
                    data_float = (float)(*((double*)buf_cl));
                    for (int j = 0; j < 4; j++)
                    {
                        *buf_mem = *((char*)&data_float + j);
                        buf_mem++;
                    }
                    buf_cl += 8;
                }
            }
            ReleaseMutex(mutex_analog_in);
        }
        goto next;
    }
    catch (...)
    {
        std::cout << "ID_CLIENT: " << init_client->id_device << "\tCLIENT_FALLED";
        logmess.c_str();
        logmess += "ID_CLIENT: " + std::to_string(init_client->id_device) + "\tCLIENT_FALLED";
        logbug->WriteLogWARNING(logmess.c_str());
    }
    end_river:
    closesocket(sock_client);
    return 0;
}




DWORD WINAPI thread_server(LPVOID config_server)
{
    config_device* init_server = (config_device*)config_server;   /// конфигурационные данные

    /// --- все для сокетов --- ///  
    SOCKET sock_server;
    SOCKADDR_IN addr_server;
    SOCKET connect_client;
    SOCKADDR_IN addr_client;
    int size_socket_client = sizeof(addr_client);   /// для accept


    /// --- для регистрации временных интервалов --- /// не так важно simintech живет по-своему
    LARGE_INTEGER end_time;
    LARGE_INTEGER start_time;
    LARGE_INTEGER freqency;
    QueryPerformanceFrequency(&freqency);
    double time;

    /// --- буферы для приема запроса --- ///
    WSABUF wsabuf_read;
    char* buf_read = new char[16];
    DWORD count_read = 0;
    DWORD flag_recv = 0;
    int number_read_value = 0;
    int command = 0;
    DWORD count_get_byte = 0;
    DWORD count_send;

    /////////////////////////////
    int num_data_from_client = 0;
    float value = 0;
    double iter = 0;
    ////////////////////////////

    /// --- буферы для ответа --- ///
    WSABUF wsabuf_write;
    int num_data = init_server->num_data;
    char* buf_write = new char[num_data * 8 + 12];
    char* buf_mem;
    char* buf_write_out;
    double data_double;
    int result_wait_mutex;
    DWORD flag_send=0;

    /// --- overlapped --- /// 
    DWORD result_wait_recv;
    DWORD result_wait_send;
    WSAOVERLAPPED send_overlapped;
    WSAOVERLAPPED recv_overlapped;
    SecureZeroMemory((PVOID)&send_overlapped, sizeof(WSAOVERLAPPED));
    SecureZeroMemory((PVOID)&recv_overlapped, sizeof(WSAOVERLAPPED));
    send_overlapped.hEvent = WSACreateEvent();
    recv_overlapped.hEvent = WSACreateEvent();

    /// --- для хранения ошибки --- ///
    int last_error = 0;

    /// --- инийиализация WSABUF --- /// 
    wsabuf_read.len = 16;
    wsabuf_read.buf = buf_read;
    wsabuf_write.len = num_data * 8 + 12;
    wsabuf_write.buf = buf_write;

    std::string logmess;

    try
    {
        sock_server = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (sock_server == INVALID_SOCKET)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_INITIALIZATION_SOCKET CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_INITIALIZATION_SOCKET CODE_ERRR :" + std::to_string(last_error);
            logbug->WriteLogERR(logmess.c_str());
            Sleep(2000);
            return -1;
        }

        addr_server.sin_addr.s_addr = inet_addr(init_server->ip_address.c_str());
        addr_server.sin_port = htons(init_server->port);
        addr_server.sin_family = AF_INET;

        if (bind(sock_server, (sockaddr*)&addr_server, sizeof(addr_server)) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_BIND_SOCKET ID_SERVER: CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_BIND_SOCKET CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogERR(logmess.c_str());
            closesocket(sock_server);
            Sleep(2000);
            return -1;
        }

        listen(sock_server, 1);

    next_client_simintech:
        if (flag_end == 1) goto end_river;
        connect_client = accept(sock_server, (sockaddr*)&addr_client, &size_socket_client);
        if (connect_client == INVALID_SOCKET)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_CONNECT_WITH_CLIENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_CONNECT_WITH_CLIENT CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }
        else
        {
            std::cout << "ID_SERVER: " << init_server->id_device << "\tCONNECT_WITH_CLIENT_IP: "
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b1 << "."
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b2 << "."
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b3 << "."
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b4
                << "  PORT: " << addr_client.sin_port << std::endl;

            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tCONNECT_WITH_CLIENT_IP: "
                + std::to_string((int)addr_client.sin_addr.S_un.S_un_b.s_b1) + "."
                + std::to_string((int)addr_client.sin_addr.S_un.S_un_b.s_b2) + "."
                + std::to_string((int)addr_client.sin_addr.S_un.S_un_b.s_b3) + "."
                + std::to_string((int)addr_client.sin_addr.S_un.S_un_b.s_b4) + " "
                + "PORT: " + std::to_string(addr_client.sin_port);
            logbug->WriteLogINFO(logmess.c_str());
        }
        QueryPerformanceCounter(&start_time);

    next:
        if (flag_end == 1) goto end_river;
        QueryPerformanceCounter(&end_time);
        time = (end_time.QuadPart - start_time.QuadPart) * 1000.0 / freqency.QuadPart;
        if (time > 100)
        {
            std::cout << "ID_SERVER: " << init_server->id_device << "\tLIMIT_TIME_MESSENG_WRITING_EXCEEDED time -" << time << "ms " << get_time_local() << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tLIMIT_TIME_MESSENG_WRITING_EXCEEDED time -" + std::to_string(time) + "ms";
            logbug->WriteLogWARNING(logmess.c_str());
        }
        count_read = 0;
    read_messeng:
        wsabuf_read.buf = buf_read + count_read;
        wsabuf_read.len = 16 - count_read;
        count_get_byte = 0;
        flag_recv = 0;
        if (WSARecv(connect_client, &wsabuf_read, 1, &count_get_byte, &flag_recv, &recv_overlapped, NULL) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            if (last_error != WSA_IO_PENDING)
            {
                std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_RECV CODE_ERROR: " << last_error << std::endl;
                logmess.clear();
                logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_RECV CODE_ERROR: " + std::to_string(last_error);
                logbug->WriteLogWARNING(logmess.c_str());
                closesocket(connect_client);
                goto next_client_simintech;
            }
        }

        result_wait_recv = WSAWaitForMultipleEvents(1, &recv_overlapped.hEvent, TRUE, INFINITE, TRUE);
        last_error = WSAGetLastError();
        if (result_wait_recv == WSA_WAIT_FAILED)
        {
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_WAIT_EVENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_WAIT_EVENT CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }
        WSAResetEvent(recv_overlapped.hEvent);

        if (!WSAGetOverlappedResult(connect_client, &recv_overlapped, &count_get_byte, FALSE, &flag_recv))
        {
            last_error = WSAGetLastError();
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_READ CODE_ERROR:" << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_READ CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }

        count_read += count_get_byte;
        if (count_read == 0 && last_error != WSA_IO_PENDING)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_EXCHANGE_WITH_CLIENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_EXCHANGE_WITH_CLIENT CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }
        if (count_read == 4 || count_read == 8)
        {
            command = *((int*)&buf_read[0]);
            if (command == 4 || command == 1)
            {
                std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR CLIENT_DISCONNECTED PORT: " << init_server->port << " " << get_time_local() << std::endl;
                logmess.clear();
                logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR CLIENT_DISCONNECTED PORT:" + std::to_string(init_server->port);
                logbug->WriteLogWARNING(logmess.c_str());
                closesocket(connect_client);
                goto next_client_simintech;
            }
        }
        if (count_read < 16) goto read_messeng;
        num_data_from_client = *((int*)&buf_read[12]);

        if (num_data_from_client != 0)
        {
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_EXCHANGE_WITH_CLIENT: CLIENT_TRANSFER_DATA" << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_EXCHANGE_WITH_CLIENT: CLIENT_TRANSFER_DATA";
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }

        QueryPerformanceCounter(&start_time);

        for (int i = 0; i < 4; i++)
        {
            buf_write[i] = *((char*)&num_data + i);
        }

        if (init_server->type_data == "analog")
        {
            buf_mem = buf_analog_out + init_server->offset * sizeof(float);
            buf_write_out = buf_write + 12;
            result_wait_mutex = WaitForSingleObject(mutex_analog_out, 4);
            if (result_wait_mutex == WAIT_OBJECT_0)
            {
                for (int i = 0; i < num_data; i++)
                {
                    data_double = (double)*((float*)buf_mem);
                    for (int j = 0; j < 8; j++)
                    {
                        *buf_write_out = *((char*)&data_double + j);
                        buf_write_out++;
                    }
                    buf_mem += 4;
                }
                ReleaseMutex(mutex_analog_out);
            }
        }

        if (init_server->type_data == "discrete")
        {
            buf_mem = buf_discrete_out + init_server->offset * sizeof(int);
            buf_write_out = buf_write + 12;
            result_wait_mutex = WaitForSingleObject(mutex_discrete_out, 4);
            if (result_wait_mutex == WAIT_OBJECT_0)
            {
                for (int i = 0; i < num_data; i++)
                {
                    data_double = (double)*((int*)buf_mem);
                    for (int j = 0; j < 8; j++)
                    {
                        *buf_write_out = *((char*)&data_double + j);
                        buf_write_out++;
                    }
                    buf_mem += 4;
                }
                ReleaseMutex(mutex_discrete_out);
            }
        }

        if (WSASend(connect_client, &wsabuf_write, 1, &count_send, flag_send, &send_overlapped, NULL) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            if (last_error != WSA_IO_PENDING)
            {
                std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_SEND CODE_ERROR: " << last_error << std::endl;
                logmess.clear();
                logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_SEND CODE_ERROR: " + std::to_string(last_error);
                logbug->WriteLogWARNING(logmess.c_str());
                if (last_error == 10035 || last_error == 10038 || last_error == 10050 || last_error == 10051 || last_error == 10052 ||
                    last_error == 10053 || last_error == 10054 || last_error == 10057 || last_error == 10058 || last_error == 10053 ||
                    last_error == 10061 || last_error == 10064 || last_error == 10065 || last_error == 10101)
                {
                    closesocket(connect_client);
                    goto next_client_simintech;
                }
            }

        }
        result_wait_send = WSAWaitForMultipleEvents(1, &send_overlapped.hEvent, TRUE, INFINITE, TRUE);
        if (result_wait_send == WSA_WAIT_FAILED)
        {
            last_error = WSAGetLastError();
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_WAIT_EVENT CODE_ERROR: " << last_error << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_WAIT_EVENT CODE_ERROR: " + std::to_string(last_error);
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }
        WSAGetOverlappedResult(connect_client, &send_overlapped, &count_send, FALSE, &flag_send);
        if (count_send != num_data * 8 + 12)
        {
            std::cout << "ID_SERVER: " << init_server->id_device << "\tERROR_TRANSFER_DATA" << std::endl;
            logmess.clear();
            logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tERROR_TRANSFER_DATA";
            logbug->WriteLogWARNING(logmess.c_str());
            closesocket(connect_client);
            goto next_client_simintech;
        }
        goto next;
    }
    catch (...)
    {
        std::cout << "ID_SERVER: " << init_server->id_device << "\tSERVER_FALLED" << std::endl;
        logmess.c_str();
        logmess += "ID_SERVER: " + std::to_string(init_server->id_device) + "\tSERVER_FALLED";
        logbug->WriteLogWARNING(logmess.c_str());
    }  
    end_river:
    closesocket(connect_client);
    closesocket(sock_server);
    return 0;
}

BOOL WINAPI close_prog(DWORD fdwCtrlType)
{
    flag_end = 1;
    Sleep(3000);
    for (auto iter = v_adapters.begin(); iter != v_adapters.end(); iter++)
    {
        TerminateThread(iter->hand_device, NULL);
    }
    return 0;
}

std::string get_time_local()
{
    char t[14];
    SYSTEMTIME time;
    GetLocalTime(&time);
    sprintf(t, "\t%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    std::string ss = t;
    return ss;
}

int init_struct_shared_memory(std::string str)
{
    int pos[2] = { 0, 0};
    std::string helpstr1;
    std::string helpstr2;
    pos[0] = str.find('\t', 0);
    if (pos[0] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    pos[1] = str.find('\t', pos[0] + 1);
    if (pos[1] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    helpstr1 = str.substr(pos[0]+1, pos[1] - pos[0]-1);
    helpstr2 = str.substr(pos[1] + 1);
    if (str.find("InputDiscrete") != -1)
    {        
        Sharmem_InDiscrete.size = atoi(helpstr1.c_str());       
        Sharmem_InDiscrete.name = helpstr2;
        return 0;
    }
    if (str.find("InputAnalog") != -1)
    {
        Sharmem_InAnalog.size = atoi(helpstr1.c_str());       
        Sharmem_InAnalog.name = helpstr2;
        return 0;
    }
    if (str.find("OutputDiscrete") != -1)
    {
       
        Sharmem_OutDiscrete.size = atoi(helpstr1.c_str());       
        Sharmem_OutDiscrete.name = helpstr2;
        return 0;
    }

    if (str.find("OutputAnalog") != -1)
    {        
        Sharmem_OutAnalog.size = atoi(helpstr1.c_str());
        Sharmem_OutAnalog.name = helpstr2;
        return 0;
    }
    std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
    logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
    return -1;
}

int init_struct_config_device(std::string str)
{
    config_device device;
    std::string helpstr1;
    std::string helpstr2;
    device.id_device = count_adapt;
    count_adapt++;
    int pos[2] = { 0, 0 };

    pos[0] = str.find('\t', 0);
    if (pos[0] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        system("pause");
        return -1;
    }
    pos[0]++;
    pos[1] = str.find('\t', pos[0]);
    if (pos[1] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.offset = atoi(helpstr1.c_str());

    pos[0] = pos[1]+1;
    pos[1] = str.find('\t', pos[0]);
    if (pos[1] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.num_data = atoi(helpstr1.c_str());

    pos[0] = pos[1]+1;
    pos[1] = str.find('\t', pos[0]);
    if (pos[1] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.ip_address = helpstr1;

    pos[0] = pos[1]+1;
    pos[1] = str.find('\t', pos[0]);
    if (pos[1] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.port = atoi(helpstr1.c_str());

    pos[0] = pos[1]+1;
    helpstr1 = str.substr(pos[0]);
    if (pos[1] == -1)
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    device.freqency = atoi(helpstr1.c_str());

    if (str.find("Analog") != -1)
    {
        device.type_data = "analog";
    }
    else if (str.find("Discrete") != -1)
    {
        device.type_data = "discrete";
    }
    else
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }

    if (str.find("Input") != -1)
    {
        device.type_device = "Server";
    }
    else if (str.find("Output") != -1)
    {
        device.type_device = "Client";
    }
    else
    {
        std::cout << "MAINT\tERROR_FORMATION_OF_CONFIG_FILE" << std::endl;
        logbug->WriteLogERR("MAINT\tERROR_FORMATION_OF_CONFIG_FILE");
        return -1;
    }
    v_adapters.push_back(device);
    return 0;
}