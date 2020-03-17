#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <list>
#include<TCHAR.H>


/// --- кол-во данных в общей памяти --- ///
#define EMT_DISCRETE_IN 100
#define EMT_DISCRETE_OUT 100
#define EMT_ANALOG_IN 100
#define EMT_ANALOG_OUT 100

#define T_FORWARD_DEFAULT 90;
#define TIMEOUT_RECV 2000;
#define MESSENG_BUFFER 10
#define TIME_DIV 25



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
std::string messeng_file[MESSENG_BUFFER + 5];
FILE* log_file = NULL;
int count_messeng = 0;
int flag_fullbuf = 0;
std::string helpstr;
int cc = 0;
void KEEP_A_DIARY();

SOCKET *mass_sock[4];


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
};
std::list <config_device> v_adapters;

struct config_memory
{
    std::string name;
    int size=0;
};

config_memory Sharmem_InDiscrete;
config_memory Sharmem_InAnalog;
config_memory Sharmem_OutDiscrete;
config_memory Sharmem_OutAnalog;

config_device adapters[4];
int count_adapt = 0;
int count_server = 0;
int count_client = 0;

/// --- вспомогательные функии --- ///
int init_struct_config_device(std::string str);
void init_struct_shared_memory(std::string str);
std::string get_time_local();
void write_to_log_file(); // записть в лог файл;
void form_string(const char* str, int f_time = 0, unsigned long value = 0);

/// --- потоки SERVER CLIENT --- ///
void thread_client(LPVOID config_client);
void thread_server(LPVOID config_server);

/// --- обработчик аварийного закрытия программы ---///
LPTOP_LEVEL_EXCEPTION_FILTER old_handler = 0;
LONG WINAPI handler_crash(PEXCEPTION_POINTERS pExceptionInfo);
BOOL WINAPI close_prog(DWORD fdwCtrlType);

int main()
{
    std::string asdqwe = get_time_local();
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    GetPriorityClass(GetCurrentProcess());
    old_handler = SetUnhandledExceptionFilter(handler_crash);
    SetConsoleCtrlHandler(close_prog, TRUE);
    HANDLE handle_window = GetConsoleWindow();
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        std::cout << "ERROR_WSAStartup" << std::endl;
        return 0;
    }

	/// --- считывание инф с конфиг файла --- ///
    ///  --- config_ver2 --- ///
    config_file = fopen("config_ver2.txt", "r");
    if (config_file == NULL)
    {
        std::cout << "ERROR_OPENING_CONFIG_FILE" << std::endl;
        return -1;
    }
    
    
    while (1)
    {
       
        while (simvol != '\n' && res_read!=EOF)
        {
            res_read = fscanf(config_file, "%c", &simvol);
            if (simvol > 0x1F && res_read != EOF) str_info += simvol;
        }
        if (str_info == "[List]" || res_read == EOF) break;
        if (str_info.substr(0, 4) == "@EMT")
        {            
            init_struct_shared_memory(str_info);
        }
        if (str_info.substr(0, 4) == "@RAE")
        {
             init_struct_config_device(str_info);
        }
        str_info.clear();
        simvol = 0;
    }
    fclose(config_file);
       
	/// --- инициализация общей памяти --- /// 

	TCHAR muxdisout[] = TEXT("mutex_discrete_out");
	TCHAR muxdisin[] = TEXT("mutex_discrete_in");
	TCHAR muxanalogout[] = TEXT("mutex_analog_out");
	TCHAR muxanalogin[] = TEXT("mutex_analog_in");

	mutex_discrete_out = CreateMutex(NULL, FALSE, muxdisout);
	mutex_analog_out = CreateMutex(NULL, FALSE, muxanalogout);
	sharmemory_emt_discrete_out = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_OutDiscrete.size*4, Sharmem_OutDiscrete.name.c_str());
	sharmemory_emt_analog_out = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_OutAnalog.size * 4, Sharmem_OutAnalog.name.c_str());
	buf_discrete_out = (char*)MapViewOfFile(sharmemory_emt_discrete_out, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_OutDiscrete.size * 4);
	buf_analog_out = (char*)MapViewOfFile(sharmemory_emt_analog_out, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_OutAnalog.size * 4);

	mutex_discrete_in = CreateMutex(NULL, FALSE, muxdisin);
	mutex_analog_in = CreateMutex(NULL, FALSE, muxanalogin);
	sharmemory_emt_discrete_in = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_InDiscrete.size * 4, Sharmem_InDiscrete.name.c_str());
	sharmemory_emt_analog_in = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, Sharmem_InAnalog.size * 4, Sharmem_InAnalog.name.c_str());
	buf_discrete_in = (char*)MapViewOfFile(sharmemory_emt_discrete_in, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_InDiscrete.size * (int)4);
	buf_analog_in = (char*)MapViewOfFile(sharmemory_emt_analog_in, FILE_MAP_ALL_ACCESS, 0, 0, Sharmem_InAnalog.size * (int)4);

	/// --- инициализацая потоков SERVER CLIENT --- ///
   
    for (std::list<config_device>::iterator iter_list = v_adapters.begin(); iter_list != v_adapters.end(); iter_list++)
    {
          config_device adapt = *iter_list;
          if (adapt.type_device == "Client")
          {
              CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_client, &(*iter_list), NULL, NULL);
          }
          if (adapt.type_device == "Server")
          {
              CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_server, &(*iter_list), NULL, NULL);
          }
    }
next:
	Sleep(2000);
	
    /*for (int i = 0; i < 2; i++)
    {
        std::cout << *(int*)(buf_discrete_in+i*4) << std::endl;
        std::cout << *(float*)(buf_analog_in + i * 4) << std::endl;
        std::cout << "........." << std::endl;
    }*/
	goto next;

	return 0;
}


void thread_client(LPVOID config_client)
{
	/// --- инициализация параметров клиента --- /// 
    config_device* init_client = (config_device*)config_client;
    SOCKET sock_client;
    SOCKADDR_IN adr_server;
    mass_sock[init_client->id_device] = &sock_client; // на будущее но хз ...   
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

    /// --- иниц пакета запроса --- ///
    int num_data = init_client->num_data;
    char* buf_read = new char[num_data * 8];
    WSABUF wsarecv;
    DWORD count_recv = 0;
    DWORD flag_recv = 0;
    DWORD result_wait_recv;
    WSAOVERLAPPED recv_overlapped;
    SecureZeroMemory((PVOID)&recv_overlapped, sizeof(WSAOVERLAPPED));
    recv_overlapped.hEvent = WSACreateEvent();
    wsarecv.buf = buf_read;
    wsarecv.len = num_data*8;
    
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
 
    for (int i = 0; i < num_data * 8; i++) buf_read[i] = 0;

    /// --- соединение с сервером --- ///
    repeat_connect:
    sock_client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL,0, WSA_FLAG_OVERLAPPED);
    if (sock_client == INVALID_SOCKET)
    {
        std::cout << "ERROR_INVALID_SOCKET ID_CLIENT: "<<init_client->id_device << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        return;
    }
    
    adr_server.sin_addr.s_addr = inet_addr(init_client->ip_address.c_str());
    adr_server.sin_port = htons(init_client->port);
    adr_server.sin_family = AF_INET;

    connect_server:
    if (connect(sock_client, (sockaddr*)&adr_server, sizeof(adr_server)) == SOCKET_ERROR)
    {
        std::cout << "ERROR_CONNECT_WITH_SERVER: PORT " << ntohs(adr_server.sin_port) << std::endl;
        std::cout << "ERROR - " << WSAGetLastError() << std::endl;
        Sleep(5000);
        goto connect_server;
    }
    else
    {
        std::cout << "CONNECT_WITH_SERVER_DONE: PORT " << ntohs(adr_server.sin_port) << std::endl;
    }

    /// --- формирование запроса --- ///
    memset(buf_request, 0, 16);
    buf_request[0] = 2;
    QueryPerformanceCounter(&time_last_messeng);
    ///  --- отправка запроса --- ///
    next:
    wwait:
    Sleep(1);
    QueryPerformanceCounter(&time_river_read);
    time = (time_river_read.QuadPart - time_last_messeng.QuadPart) * 1000.0 / freqency.QuadPart;
    if (time < (init_client->freqency-TIME_DIV)) goto wwait;
    if (time > init_client->freqency)
    {
        std::cout << "LIMIT_TIME_MESSENG_READING_EXCEEDED ID - " << init_client->id_device << " " << time << get_time_local() << std::endl;
    }
       
    /// --- отправка запроса на чтение --- ///
    repeat_send_read:
    QueryPerformanceCounter(&time_last_messeng);

    if (WSASend(sock_client, &wsasend, 1, &count_send, flag_send, &send_overlapped, NULL) == SOCKET_ERROR)
    {
        last_error = WSAGetLastError();
        if (last_error != WSA_IO_PENDING)
        {
            std::cout << "ERROR_SEND" << std::endl;
            std::cout << last_error << std::endl;
            if (last_error == 10035 || last_error == 10038 || last_error == 10050 || last_error == 10051 || last_error == 10052 ||
                last_error == 10053 || last_error == 10054 || last_error == 10057 || last_error == 10058 || last_error == 10053 ||
                last_error == 10061 || last_error == 10064 || last_error == 10065 || last_error == 10101)
            {
                sleep_time = 2000;
             reconnect:
                std::cout << "RECONNECT..." << std::endl;
                closesocket(sock_client);
                sock_client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
                if (connect(sock_client, (sockaddr*)&adr_server, sizeof(adr_server)) == SOCKET_ERROR)
                {
                    std::cout << "ERROR_CONNECTING_PORT " << ntohs(adr_server.sin_port) << get_time_local() << std::endl;
                    std::cout << "ERROR - " << WSAGetLastError() << std::endl;
                    Sleep(sleep_time);
                    sleep_time += sleep_time;
                    if (sleep_time >= 60000) sleep_time = 2000;
                    goto reconnect;
                }
                std::cout << "CONNECT_WITH_SERVER_DONE: PORT " << ntohs(adr_server.sin_port) << get_time_local() << std::endl;
                goto repeat_send_read;
            }
        }        
    }
    result_wait_send = WSAWaitForMultipleEvents(1, &send_overlapped.hEvent, TRUE, INFINITE, TRUE);
    if (result_wait_send == WSA_WAIT_FAILED)
    {
        std::cout << "ERROR_WAIT_EVENT" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        closesocket(sock_client);
        goto repeat_connect;
    }
    WSAGetOverlappedResult(sock_client, &send_overlapped, &count_send, FALSE, &flag_send);
    if (count_send != 16)
    {
        std::cout << "ERROR_REQEST" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        closesocket(sock_client);
        goto repeat_connect;
    }

    /// --- прием сообщения --- /// 

    if (WSARecv(sock_client, &wsarecv, 1, &count_recv, &flag_recv, &recv_overlapped, NULL) == SOCKET_ERROR)
    {

    }


    goto next;

    /// --- прием данных --- ///

    



    count_buf = 0;
    /// --- чтение первых 12-байт (команды)--- ///
    do 
    {
        count_buf += recv(sock_client, buf_read + count_buf, 12 - count_buf, NULL);
    } while (count_buf < 12 && count_buf != SOCKET_ERROR);

    /// ---  чтение кол-ва читаемых данных --- ///
    num_data_from_server = *((int*)buf_read);                                           
    num_data_from_server = num_data_from_server * sizeof(double);

    //// ---- читаем данные и пишим их в буфер обмена --- ///
    count_buf = 0;
    do
    {
        count_buf += recv(sock_client, buf_read + count_buf, num_data_from_server - count_buf, NULL);
    } while (count_buf < num_data_from_server && count_buf != SOCKET_ERROR);

    if (init_client->type_data == "discrete")
    {
        result_wait_mutex = WaitForSingleObject(mutex_discrete_in, 4);
        if (result_wait_mutex == WAIT_OBJECT_0)
        {
            buf_mem = buf_discrete_in;
            buf_cl = buf_read;
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
            buf_mem = buf_analog_in;
            buf_cl = buf_read;
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




void thread_server(LPVOID config_server)
{
    config_device* init_server = (config_device*)config_server;   /// конфигурационные данные

    /// --- все для сокетов --- ///  
    SOCKET sock_server;
    SOCKADDR_IN addr_server;
    SOCKET connect_client;
    SOCKADDR_IN addr_client;
    int size_socket_client = sizeof(addr_client);   /// для accept
    mass_sock[init_server->id_device] = &connect_client;  /// временно для закрытия сокетов  


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
    int zz = 0;
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



    /// --- инициализация WSA-сокета --- ///
    sock_server = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (sock_server == INVALID_SOCKET)
    {
        std::cout << "ERROR_INITIALIZATION_SOCKET" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        return;
    }

    addr_server.sin_addr.s_addr = inet_addr(init_server->ip_address.c_str());
    addr_server.sin_port = htons(init_server->port);
    addr_server.sin_family = AF_INET;

    if (bind(sock_server, (sockaddr*)&addr_server, sizeof(addr_server)) == SOCKET_ERROR)
    {
        std::cout << "ERROR_BIND_SOCKET ID_SERVER: " << init_server->id_device << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        closesocket(sock_server);
        return;
    }

    listen(sock_server, 1);

next_client_simintech:

    connect_client = accept(sock_server, (sockaddr*)&addr_client, &size_socket_client);
    if (connect_client == INVALID_SOCKET)
    {
        std::cout << "ERROR_CONNECT_WITH_CLIENT" << std::endl;
        closesocket(connect_client);
        goto next_client_simintech;
    }
    else
    {
        std::cout << "CONNECT_WITH_CLIENT_IP: "
            << (int)addr_client.sin_addr.S_un.S_un_b.s_b1 << "."
            << (int)addr_client.sin_addr.S_un.S_un_b.s_b2 << "."
            << (int)addr_client.sin_addr.S_un.S_un_b.s_b3 << "."
            << (int)addr_client.sin_addr.S_un.S_un_b.s_b4
            << "  PORT: " << addr_client.sin_port << std::endl;
    }
    QueryPerformanceCounter(&start_time);

next:
    QueryPerformanceCounter(&end_time);
    time = (end_time.QuadPart - start_time.QuadPart) * 1000.0 / freqency.QuadPart;
    if (time > 100)
    {
        std::cout << "LIMIT_TIME_MESSENG_WRITING_EXCEEDED ID - " << init_server->id_device << "\t" << time << get_time_local() << std::endl;
    }
    count_read = 0;
read_messeng:
    wsabuf_read.buf = buf_read+count_read;
    wsabuf_read.len = 16-count_read;
    count_get_byte = 0;
    flag_recv = 0;
    if (WSARecv(connect_client, &wsabuf_read, 1, &count_get_byte, &flag_recv, &recv_overlapped, NULL) == SOCKET_ERROR)
    {
        last_error = WSAGetLastError();
        if (last_error != WSA_IO_PENDING)
        {
            std::cout << "ERROR_RECV" << std::endl;
            std::cout << last_error << std::endl;
            closesocket(connect_client);
            goto next_client_simintech;
        }       
    }

    result_wait_recv = WSAWaitForMultipleEvents(1, &recv_overlapped.hEvent, TRUE, INFINITE, TRUE);
    last_error = WSAGetLastError();
    if (result_wait_recv == WSA_WAIT_FAILED)
    {
        std::cout << "ERROR_WAIT_EVENT" << std::endl;
        std::cout << last_error << std::endl;
        closesocket(connect_client);
        goto next_client_simintech;
    }    
    WSAResetEvent(recv_overlapped.hEvent);

    if (!WSAGetOverlappedResult(connect_client, &recv_overlapped, &count_get_byte, FALSE, &flag_recv))
    {
        last_error = WSAGetLastError();
        std::cout << "ERROR_READ_SERVER ID - "<<init_server->id_device << std::endl;
        std::cout << last_error << std::endl;
        closesocket(connect_client);
        goto next_client_simintech;
    }

    count_read += count_get_byte;
    if (count_read == 0 && last_error!=WSA_IO_PENDING)
    {
        std::cout << "ERROR_EXCHANGE_WITH_CLIENT" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        closesocket(connect_client);
        goto next_client_simintech;
    }
    if (count_read == 4 || count_read == 8)
    {
        command = *((int*)&buf_read[0]);
        if (command == 4 || command == 1)
        {
            std::cout << "ERROR_CLIENT_DISCONNECTED_PORT: " << addr_client.sin_port << "  " << get_time_local() << std::endl;
            closesocket(connect_client);
            goto next_client_simintech;
        }       
    } 
    if (count_read < 16) goto read_messeng;
    num_data_from_client = *((int*)&buf_read[12]);

    if (num_data_from_client !=0)
    {
        std::cout << "ERROR_EXCHANGE_WITH_CLIENT: CLIENT_TRANSFER_DATA" << std::endl;
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
        buf_mem = buf_analog_out+init_server->offset*sizeof(float);
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
        buf_mem = buf_discrete_out+init_server->offset * sizeof(int);
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
   
    if (WSASend(connect_client, &wsabuf_write, 1, &count_send, flag_send, &send_overlapped,NULL) == SOCKET_ERROR)
    {
        last_error = WSAGetLastError();
        if (last_error != WSA_IO_PENDING)
        {
            std::cout << "ERROR_SEND" << std::endl;
            std::cout << last_error << std::endl;
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
        std::cout << "ERROR_WAIT_EVENT" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        closesocket(connect_client);
        goto next_client_simintech;
    }
    WSAGetOverlappedResult(connect_client, &send_overlapped, &count_send, FALSE, &flag_send);
    if (count_send != num_data * 8 + 12)
    {
        std::cout << "ERROR_TRANSFER_DATA" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
        closesocket(connect_client);
        goto next_client_simintech;
    }
    goto next;

}

LONG WINAPI handler_crash(PEXCEPTION_POINTERS pExceptionInfo)
{
    write_to_log_file();
    form_string("WTF  O_O", 1);
    switch (pExceptionInfo->ExceptionRecord->ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
        form_string("LAST_ERROR: EXCEPTION_ACCESS_VIOLATION");
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        form_string("LAST_ERROR: EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
        break;
    case EXCEPTION_BREAKPOINT:
        form_string("LAST_ERROR: EXCEPTION_BREAKPOINT");
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        form_string("LAST_ERROR: EXCEPTION_DATATYPE_MISALIGNMENT");
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        form_string("LAST_ERROR: EXCEPTION_FLT_DENORMAL_OPERAND");
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        form_string("LAST_ERROR: EXCEPTION_FLT_DIVIDE_BY_ZERO");
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        form_string("LAST_ERROR: EXCEPTION_FLT_INEXACT_RESULT");
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        form_string("LAST_ERROR: EXCEPTION_FLT_INVALID_OPERATION");
        break;
    case EXCEPTION_FLT_OVERFLOW:
        form_string("LAST_ERROR: EXCEPTION_FLT_OVERFLOW");
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        form_string("LAST_ERROR: EXCEPTION_FLT_STACK_CHECK");
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        form_string("LAST_ERROR: EXCEPTION_INT_DIVIDE_BY_ZERO");
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        form_string("LAST_ERROR: EXCEPTION_FLT_UNDERFLOW");
        break;
    case EXCEPTION_GUARD_PAGE:
        form_string("LAST_ERROR: EXCEPTION_GUARD_PAGE");
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        form_string("LAST_ERROR: EXCEPTION_ILLEGAL_INSTRUCTION");
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        form_string("LAST_ERROR: EXCEPTION_IN_PAGE_ERROR");
        break;
    case EXCEPTION_INT_OVERFLOW:
        form_string("LAST_ERROR: EXCEPTION_INT_OVERFLOW");
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        form_string("LAST_ERROR: EXCEPTION_INVALID_DISPOSITION");
        break;
    case EXCEPTION_INVALID_HANDLE:
        form_string("LAST_ERROR: EXCEPTION_INVALID_HANDLE");
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        form_string("LAST_ERROR: EXCEPTION_NONCONTINUABLE_EXCEPTION");
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        form_string("LAST_ERROR: EXCEPTION_PRIV_INSTRUCTION");
        break;
    case EXCEPTION_SINGLE_STEP:
        form_string("LAST_ERROR: EXCEPTION_SINGLE_STEP");
        break;
    case EXCEPTION_STACK_OVERFLOW:
        form_string("LAST_ERROR: EXCEPTION_SINGLE_STEP");
        break;
    case STATUS_UNWIND_CONSOLIDATE:
        form_string("LAST_ERROR: EXCEPTION_NONCONTINUABLE_EXCEPTION");
        break;
    default:
        form_string("LAST_ERROR: NO INFO", 0, pExceptionInfo->ExceptionRecord->ExceptionCode);
        break;
    }
    write_to_log_file();
    if (old_handler) return old_handler(pExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}

BOOL WINAPI close_prog(DWORD fdwCtrlType)
{
    for (int i = 0; i < 4; i++) closesocket(*mass_sock[i]);
    
    write_to_log_file();
    form_string("WTF  O_O", 1);
    form_string("CLOSE_PROGRAMM");
    write_to_log_file();
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

void write_to_log_file()
{
    log_file = fopen("log.txt", "a");
    for (int i = 0; i < count_messeng; i++)
    {
        fprintf(log_file, messeng_file[i].c_str());
        messeng_file[i].clear();
    }
    fclose(log_file);
    flag_fullbuf = 0;
    count_messeng = 0;
    cc++;
    if (cc > 10) { cc = 0;  system("cls"); }
}

void form_string(const char* str, int f_time, unsigned long value)
{
    std::string out_str;
    out_str = str;
    if (value != 0)
    {
        char* val_str = new char();
        sprintf(val_str, "\t%lu", value);
        out_str += val_str;
        delete (val_str);
    }
    if (f_time != 0)
    {

        char time[14];
        SYSTEMTIME tic;
        GetLocalTime(&tic);
        sprintf(time, "\t%02d:%02d:%02d.%03d", tic.wHour, tic.wMinute, tic.wSecond, tic.wMilliseconds);
        out_str += time;
    }

    out_str += '\n';
    //messeng_file[count_messeng] = out_str;
    //count_messeng++;
    if (count_messeng < MESSENG_BUFFER)
    {
        messeng_file[count_messeng] = out_str;
        count_messeng++;
    }
    if (count_messeng >= MESSENG_BUFFER)
    {
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)KEEP_A_DIARY, NULL, NULL, NULL);
    }
}

void KEEP_A_DIARY()
{
    write_to_log_file();
}

void init_struct_shared_memory(std::string str)
{
    int pos[2] = { 0, 0};
    std::string helpstr1;
    std::string helpstr2;
    pos[0] = str.find(' ', 0);
    pos[1] = str.find(' ', pos[0] + 1);
    helpstr1 = str.substr(pos[0]+1, pos[1] - pos[0]-1);
    helpstr2 = str.substr(pos[1] + 1);
    if (str.find("InputDiscrete") != -1)
    {        
        Sharmem_InDiscrete.size = atoi(helpstr1.c_str());       
        Sharmem_InDiscrete.name = helpstr2;
        return;
    }
    if (str.find("InputAnalog") != -1)
    {
        Sharmem_InAnalog.size = atoi(helpstr1.c_str());       
        Sharmem_InAnalog.name = helpstr2;
        return;
    }
    if (str.find("OutputDiscrete") != -1)
    {
       
        Sharmem_OutDiscrete.size = atoi(helpstr1.c_str());       
        Sharmem_OutDiscrete.name = helpstr2;
        return;
    }

    if (str.find("OutputAnalog") != -1)
    {        
        Sharmem_OutAnalog.size = atoi(helpstr1.c_str());
        Sharmem_OutAnalog.name = helpstr2;
        return;
    }

}

int init_struct_config_device(std::string str)
{
    config_device device;
    std::string helpstr1;
    std::string helpstr2;
    device.id_device = count_adapt;
    count_adapt++;
    int pos[2] = { 0, 0 };

    pos[0] = str.find(' ', 0);
    if (pos[0] == -1) return -1;
    pos[0]++;
    pos[1] = str.find(' ', pos[0]);
    if (pos[1] == -1) return -1;
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.offset = atoi(helpstr1.c_str());

    pos[0] = pos[1]+1;
    pos[1] = str.find(' ', pos[0]);
    if (pos[1] == -1) return -1;
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.num_data = atoi(helpstr1.c_str());

    pos[0] = pos[1]+1;
    pos[1] = str.find(' ', pos[0]);
    if (pos[1] == -1) return -1;
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.ip_address = helpstr1;

    pos[0] = pos[1]+1;
    pos[1] = str.find(' ', pos[0]);
    if (pos[1] == -1) return -1;
    helpstr1 = str.substr(pos[0], pos[1] - pos[0]);
    device.port = atoi(helpstr1.c_str());

    pos[0] = pos[1]+1;
    helpstr1 = str.substr(pos[0]);
    if (pos[1] == -1) return -1;
    device.freqency = atoi(helpstr1.c_str());

    if (str.find("Analog") != -1)
    {
        device.type_data = "analog";
    }
    else if (str.find("Discrete") != -1)
    {
        device.type_data = "discrete";
    }
    else return -1;

    if (str.find("Input") != -1)
    {
        device.type_device = "Server";
    }
    else if (str.find("Output") != -1)
    {
        device.type_device = "Client";
    }
    else return -1;
    v_adapters.push_back(device);
    return 0;
}