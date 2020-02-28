//#pragma once
#ifndef __SHAREDMEMORY_H_
#define __SHAREDMEMORY_H_
#include <windows.h>
#define EMT_DISCRETE_IN 100
#define EMT_DISCRETE_OUT 100
#define EMT_ANALOG_IN 100
#define EMT_ANALOG_OUT 100

#define EMT_TO_RAEK_ANALOG     100  // размер массива аналоговых значений всех стоек, передаваемых в –јЁ 
#define EMT_FROM_RAEK_ANALOG   100  // размер массива аналоговых значений всех стоек, принимаемых от –јЁ 
#define EMT_TO_RAEK_DISCRETE   100  // размер массива дискретных значений всех стоек, передаваемых в –јЁ 
#define EMT_FROM_RAEK_DISCRETE 100  // размер массива дискретных значений всех стоек, принимаемых от –јЁ 

extern HANDLE sharmemory_emt_discrete_in;
extern HANDLE sharmemory_emt_analog_in;
extern HANDLE sharmemory_emt_discrete_out;
extern HANDLE sharmemory_emt_analog_out;
extern HANDLE mutex_discrete_out;
extern HANDLE mutex_analog_out;
extern HANDLE mutex_discrete_in;
extern HANDLE mutex_analog_in;
extern int* buf_discrete_out;
extern float* buf_analog_out;
extern int* buf_discrete_in;
extern float* buf_analog_in;

void InitSharedMemory();
void read_alldata_from_raek(int *buf_emt_discrete, float* buf_emt_analog);
void write_alldata_to_raek(int* buf_emt_discrete, float* buf_emt_analog);
void read_analog_data_from_raek(float* buf_emt);
void write_analog_data_to_raek(float* buf_emt);
void read_discrete_data_from_raek(int *buf_emt);
void write_discrete_data_to_raek(int *buf_emt);
#endif
