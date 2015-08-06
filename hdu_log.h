#ifndef _HUD_LOG_H_
#define _HDU_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * the two parameters is define in main.c
 * FILE* debug_fp = NULL;
 * char debug_time[25];
 */

void InitDebug(void);
void GetDebugTime(void);
inline void SetDebugFp(char* path);
inline void ClearDebugFp(void);

#define DEBUG_INFO(msg) \
	GetDebugTime(); \
	fprintf(debug_fp, "%s [%s] \"%s\" -> func: %s, line: %d\n", debug_time, "info", msg, __FUNCTION__, __LINE__)
#define DEBUG_ERR(msg) \
	GetDebugTime(); \
	fprintf(debug_fp, "%s [%s] \"%s\" -> func: %s, line: %d\n", debug_time, "err", msg, __FUNCTION__, __LINE__)
#define DEBUG_PRINT(msg) \
	GetDebugTime(); \
	fprintf(debug_fp, "\n----------- %s %s -----------\n\n", debug_time, msg)

#endif
