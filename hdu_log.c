#include "hdu_log.h"

extern FILE* debug_fp;
extern char debug_time[25];

static time_t debug_t;
static char* debug_tp;
static FILE* debug_path;

void InitDebug(void)
{
	debug_fp = stderr;
}

void GetDebugTime(void)
{
	if (!debug_fp)
	{
		printf("The debug_fp isn't init\n");
		return;
	}
	time_t debug_t = time(NULL);
	char* debug_tp = ctime(&debug_t);
	strcpy(debug_time, debug_tp);
	debug_time[strlen(debug_time) - 1] = '\0';
}

inline void SetDebugFp(char* path)
{
	if (!debug_fp)
	{
		printf("The debug_fp isn't init\n");
		return;
	}
	debug_path = fopen(path, "a+");
	debug_fp = debug_path;
}
inline void ClearDebugFp(void)
{
	if (!debug_fp)
	{
		printf("The debug_fp isn't init\n");
		return;
	}
	debug_fp = stderr;
}
