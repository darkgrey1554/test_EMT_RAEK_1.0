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
	sharmemory_emt_discrete_in = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_DISCRETE_IN * 4, sharmemorydisout);
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

	float z = 1.1;
next:
	Sleep(2000);
	
	buf_analog_in[0] = (char)(*((char*)&z));
	buf_analog_in[1] = (char)(*((char*)&z+1));
	buf_analog_in[2] = (char)(*((char*)&z+2));
	buf_analog_in[3] = (char)(*((char*)&z+3));
	z++;
	goto next;

	return 0;
}




void thread_client(LPVOID config_client)
{
	config_device* init_client = (config_device*)config_client;
    SOCKET sock_client;
    SOCKADDR_IN adr_server;
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

    /// --- соединение с сервером --- ///
    sock_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_client == INVALID_SOCKET)
    {
        std::cout << "ERROR_INVALID_SOCKET" << std::endl;
        std::cout << WSAGetLastError() << std::endl;
    }

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
    /*
    count_buf = 0;
    /// --- чтение первых 12-байт (команды)--- ///
    while (count_buf < 12) count_buf += recv(socket_pull, buf_read + count_buf, 12 - count_buf, NULL);

    asd = *((int*)buf_read);                                           ///// чтение кол-ва читаемых данных
    asd = asd * sizeof(double);
    //// ---- читаем данные и пишим их в буфер обмена --- ///
    count_buf = 0;
    while (count_buf < asd) count_buf += recv(socket_pull, buf_read + count_buf, asd - count_buf, NULL);

    buf_r = buf_read;
    buf_m = buf_to_client + SIZE_BUFFER * 8 * (idr);
    for (int i = 0; i < asd; i++)
    {
        *buf_m = *buf_r;
        buf_m++;
        buf_r++;
    }
    */
    goto next;
}




void thread_server(LPVOID config_server)
{
	config_device* init_server = (config_device*)config_server;
	
    next:

	Sleep(2000);
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
