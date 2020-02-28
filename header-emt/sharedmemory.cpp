#include "stdafx.h"
#include "sharedmemory.h"

HANDLE sharmemory_emt_discrete_in;
HANDLE sharmemory_emt_analog_in;
HANDLE sharmemory_emt_discrete_out;
HANDLE sharmemory_emt_analog_out;
HANDLE mutex_discrete_out;
HANDLE mutex_analog_out;
HANDLE mutex_discrete_in;
HANDLE mutex_analog_in;
int* buf_discrete_out;
float* buf_analog_out;
int* buf_discrete_in;
float* buf_analog_in;


 void InitSharedMemory()
{
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
	sharmemory_emt_discrete_out = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_TO_RAEK_DISCRETE * sizeof(int), sharmemorydisout);
	sharmemory_emt_analog_out = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_TO_RAEK_ANALOG * sizeof(float), sharmemoryanalogout);
	buf_discrete_out = (int*)MapViewOfFile(sharmemory_emt_discrete_out, FILE_MAP_ALL_ACCESS, 0, 0, EMT_TO_RAEK_DISCRETE * sizeof(int));
	buf_analog_out = (float*)MapViewOfFile(sharmemory_emt_analog_out, FILE_MAP_ALL_ACCESS, 0, 0, EMT_TO_RAEK_ANALOG * sizeof(float));

	mutex_discrete_in = CreateMutex(NULL, FALSE, muxdisin);
	mutex_analog_in = CreateMutex(NULL, FALSE, muxanalogin);
	sharmemory_emt_discrete_in = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_FROM_RAEK_DISCRETE * sizeof(int), sharmemorydisin);
	sharmemory_emt_analog_in = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, EMT_FROM_RAEK_ANALOG * sizeof(float), sharmemoryanalogin);
	buf_discrete_in = (int*)MapViewOfFile(sharmemory_emt_discrete_in, FILE_MAP_ALL_ACCESS, 0, 0, EMT_FROM_RAEK_DISCRETE * sizeof(int));
	buf_analog_in = (float*)MapViewOfFile(sharmemory_emt_analog_in, FILE_MAP_ALL_ACCESS, 0, 0, EMT_FROM_RAEK_ANALOG * sizeof(float));
}
 //////////////////////////////////////////////
 void read_alldata_from_raek(int *buf_emt_discrete, float* buf_emt_analog) 
 {
	 read_analog_data_from_raek(buf_emt_analog);
	 read_discrete_data_from_raek(buf_emt_discrete);
 }
 /////////////////////////////////////////////
 void write_alldata_to_raek(int* buf_emt_discrete, float* buf_emt_analog)
 {
	 write_analog_data_to_raek(buf_emt_analog);
	 write_discrete_data_to_raek(buf_emt_discrete);
 }
 ////////////////////////////////////////////
 void read_analog_data_from_raek(float* buf_emt)
 {
	 int result = 0;
	 result = WaitForSingleObject(mutex_analog_in, 5);
	 if (result == WAIT_OBJECT_0)
	 {
		 for (int i = 0; i < EMT_TO_RAEK_ANALOG; i++) *(buf_emt + i) = *(buf_analog_in + i);
		 ReleaseMutex(mutex_analog_in);
	 }	 
 }
 //////////////////////////////////////////////////
 void write_analog_data_to_raek(float* buf_emt)
 {
	 int result = 0;
	 result = WaitForSingleObject(mutex_analog_out, 5);
	 if (result == WAIT_OBJECT_0)
	 {
		 for (int i = 0; i < EMT_FROM_RAEK_ANALOG; i++) *(buf_analog_out+i) = *(buf_emt+i);		
		 ReleaseMutex(mutex_analog_out);
	 }	 
 }
 ///////////////////////////////////////////////////
 void read_discrete_data_from_raek(int* buf_emt)
 {
	 int result = 0;
	 result = WaitForSingleObject(mutex_discrete_in, 5);
	 if (result == WAIT_OBJECT_0)
	 {
		 for (int i = 0; i < EMT_TO_RAEK_DISCRETE; i++) *(buf_emt + i) = *(buf_discrete_in + i);
		 ReleaseMutex(mutex_discrete_in);
	 }
 }
 //////////////////////////////////////////////////
 void write_discrete_data_to_raek(int *buf_emt)
 {
	 int result = 0;
	 result = WaitForSingleObject(mutex_discrete_out, 5);
	 if (result == WAIT_OBJECT_0)
	 {
		 for (int i = 0; i < EMT_FROM_RAEK_DISCRETE; i++) *(buf_discrete_out + i) = *(buf_emt + i);
		 ReleaseMutex(mutex_discrete_out);
	 }
 }