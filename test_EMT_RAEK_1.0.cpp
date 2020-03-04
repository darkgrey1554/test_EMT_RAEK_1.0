#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <list>


/// --- кол-во данных в общей памяти --- ///
#define EMT_DISCRETE_IN 100
#define EMT_DISCRETE_OUT 100
#define EMT_ANALOG_IN 100
#define EMT_ANALOG_OUT 100

#define T_FORWARD_DEFAULT 90;
#define TIMEOUT_RECV 2000;
#define MESSENG_BUFFER 10
#define TIME_OUT 100



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
char simvol;
int num_adapters;
struct config_device
{
	std::string type_device;
	int id_device = -1;
	std::string ip_address;
	std::string port;
	std::string type_data;
	std::string num_data;
};
config_device* adapters;
int count_adapt = 0;

/// --- вспомогательные функии --- ///

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

	config_file = fopen("config.txt", "r");
	while (res_read != EOF)
	{
		res_read = fscanf(config_file, "%c", &simvol);
		if (simvol > 0x20 && res_read != EOF) str_info += simvol;
		if (simvol == '\n' && str_info.length()!=0)
		{
			config_info.push_back(str_info);
			str_info.clear();
		}
	}
	if (str_info.length()!=0) config_info.push_back(str_info);

	num_adapters = config_info.size() / 5;
	adapters = new config_device[num_adapters];

	for (auto iter = config_info.begin(); iter != config_info.end();)
	{
		adapters[count_adapt].type_device = *(iter++);
		adapters[count_adapt].ip_address = *(iter++);
		adapters[count_adapt].port = *(iter++);
		adapters[count_adapt].type_data = *(iter++);
		adapters[count_adapt].num_data = *(iter++);
		adapters[count_adapt].id_device = count_adapt;
		count_adapt++;
	}
	

	/// --- инициализация общей памяти --- /// 

	TCHAR muxdisout[] = TEXT("mutex_discrete_out");
	TCHAR muxdisin[] = TEXT("mutex_discrete_in");
	TCHAR muxanalogout[] = TEXT("mutex_analog_out");
	TCHAR muxanalogin[] = TEXT("mutex_analog_in");

	TCHAR sharmemorydisout[] = TEXT("sharmemory_emt_discrete_out");
	TCHAR sharmemorydisin[] = TEXT("sharmemory_emt_discrete_in");
	TCHAR sharmemoryanalogout[] = TEXT("sharmemory_emt_analog_out");
	TCHAR sharmemoryanalogin[] = TEXT("sharmemory_emt_analog_in");

	mutex_discrete_out = CreateMutex(NULL, FALSE, muxdisout);
	mutex_analog_out = CreateMutex(NULL, FALSE, muxanalogout);
	sharmemory_emt_discrete_out = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_DISCRETE_OUT * 4, sharmemorydisout);
	sharmemory_emt_analog_out = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_ANALOG_OUT * 4, sharmemoryanalogout);
	buf_discrete_out = (char*)MapViewOfFile(sharmemory_emt_discrete_out, FILE_MAP_ALL_ACCESS, 0, 0, EMT_DISCRETE_OUT * 4);
	buf_analog_out = (char*)MapViewOfFile(sharmemory_emt_analog_out, FILE_MAP_ALL_ACCESS, 0, 0, EMT_ANALOG_OUT * 4);

	mutex_discrete_in = CreateMutex(NULL, FALSE, muxdisin);
	mutex_analog_in = CreateMutex(NULL, FALSE, muxanalogin);
	sharmemory_emt_discrete_in = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_DISCRETE_IN * 4, sharmemorydisin);
	sharmemory_emt_analog_in = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_ANALOG_IN * 4, sharmemoryanalogin);
	buf_discrete_in = (char*)MapViewOfFile(sharmemory_emt_discrete_in, FILE_MAP_ALL_ACCESS, 0, 0, EMT_DISCRETE_IN * 4);
	buf_analog_in = (char*)MapViewOfFile(sharmemory_emt_analog_in, FILE_MAP_ALL_ACCESS, 0, 0, EMT_ANALOG_IN * 4);

	/// --- инициализацая потоков SERVER CLIENT --- ///

	for (int i = 0; i < num_adapters; i++)
	{
		if (adapters[i].type_device == "CLIENT")
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_client,&adapters[i], NULL, NULL);
		}
		if (adapters[i].type_device == "SERVER")
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)thread_server,&adapters[i], NULL, NULL);
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
	
    config_device* init_client = (config_device*)config_client;
    SOCKET sock_client;
    SOCKADDR_IN adr_server;
    mass_sock[init_client->id_device] = &sock_client;
    char* buf_request = new char[16];
    LARGE_INTEGER time_last_messeng;
    LARGE_INTEGER time_river_read;
    LARGE_INTEGER freqency;
    QueryPerformanceFrequency(&freqency);
    QueryPerformanceCounter(&time_last_messeng);
    double time;
    int result;
    int last_error;
    int sleep_time;
    int count_buf = 0;
    int num_data = atoi(init_client->num_data.c_str());
    char* buf_read = new char[num_data * 8];
    int num_data_from_server = 0;
    DWORD result_wait_mutex = 0;
    int data_int=0;
    float data_float=0.;
    char* buf_cl;
    char* buf_mem;
    char set_timeout_sock[4];
    int  set_timeout_value = 2000;


    for (int i = 0; i < num_data * 8; i++) buf_read[i] = 0;

    /// --- соединение с сервером --- ///
    sock_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_client == INVALID_SOCKET)
    {
        std::cout << "ERROR_INVALID_SOCKET ID_CLIENT: "<<init_client->id_device << std::endl;
        std::cout << WSAGetLastError() << std::endl;
    }
    set_timeout_sock[0] = (char)(*((char*)&set_timeout_value));
    set_timeout_sock[1] = (char)(*((char*)&set_timeout_value + 1));
    set_timeout_sock[2] = (char)(*((char*)&set_timeout_value + 2));
    set_timeout_sock[3] = (char)(*((char*)&set_timeout_value + 3));

    setsockopt(sock_client, SOL_SOCKET, SO_RCVTIMEO, set_timeout_sock, sizeof(DWORD));
    setsockopt(sock_client, SOL_SOCKET, SO_SNDTIMEO, set_timeout_sock, sizeof(DWORD));


    adr_server.sin_addr.s_addr = inet_addr(init_client->ip_address.c_str());
    adr_server.sin_port = htons(atoi(init_client->port.c_str()));
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

    ///  --- отправка запроса --- ///
    next:
    wwait:
    Sleep(1);
    QueryPerformanceCounter(&time_river_read);
    time = (time_river_read.QuadPart - time_last_messeng.QuadPart) * 1000.0 / freqency.QuadPart;
    if (time < 80) goto wwait;
    if (time > TIME_OUT)
    {
        std::cout << "LIMIT_TIME_MESSENG_READING_EXCEEDED ID - " << init_client->id_device << " " << time << get_time_local() << std::endl;
    }
       
    /// --- отправка запроса на чтение --- ///
    repeat_send_read:
    QueryPerformanceCounter(&time_last_messeng);
    result = send(sock_client, buf_request, 16, NULL);
    if (result == SOCKET_ERROR)
    {
        last_error = WSAGetLastError();
        std::cout << "ERROR_READING_PORT " << init_client->port << std::endl;
        std::cout << "ERROR - " << last_error << std::endl;

        if (last_error >= 10050 && last_error <= 10058 && last_error != 10055 || last_error == 10038
            || last_error == 10060 || last_error == 10061 || last_error == 10064 || last_error == 10065
            || last_error == 10108 || last_error == 10111 || last_error == 11001)
        {
        sleep_time = 2000;

        reconnect:
            std::cout << "RECONNECT..." << std::endl;
            form_string("RECONECT...");
            closesocket(sock_client);

            sock_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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
    int num_data = atoi(init_server->num_data.c_str());
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
    addr_server.sin_port = htons(atoi(init_server->port.c_str()));
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
client_sleep_wait:
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
        buf_mem = buf_analog_out;
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
        buf_mem = buf_discrete_out;
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
        }
        if (last_error == 10035 || last_error == 10038 || last_error == 10050 || last_error == 10051 || last_error == 10052 ||
            last_error == 10053 || last_error == 10054 || last_error == 10057 || last_error == 10058 || last_error == 10053 ||
            last_error == 10061 || last_error == 10064 || last_error == 10065 || last_error == 10101)
        {
            closesocket(connect_client);
            goto next_client_simintech;
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
    char t[13];
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

        char time[13];
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
