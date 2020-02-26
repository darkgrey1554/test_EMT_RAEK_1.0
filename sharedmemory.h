#pragma once
#ifndef __SHAREDMEMORY_H_
#define __SHAREDMEMORY_H_
#include <windows.h>
#define EMT_DISCRETE_IN 100
#define EMT_DISCRETE_OUT 100
#define EMT_ANALOG_IN 100
#define EMT_ANALOG_OUT 100


extern HANDLE sharmemory_emt_discrete_in;
extern HANDLE sharmemory_emt_analog_in;
extern HANDLE sharmemory_emt_discrete_out;
extern HANDLE sharmemory_emt_analog_out;
extern HANDLE mutex_discrete_out;
extern HANDLE mutex_analog_out;
extern HANDLE mutex_discrete_in;
extern HANDLE mutex_analog_in;
extern char* buf_discrete_out;
extern char* buf_analog_out;
extern char* buf_discrete_in;
extern char* buf_analog_in;

void InitSharedMemory();
#endif
