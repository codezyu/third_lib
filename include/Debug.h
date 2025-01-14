/*** Debug header. ***/

/** Version 1 + Functional Model Modification **/

/** Redundance check. **/
#ifndef DEBUG_HEADER
#define DEBUG_HEADER

/** Included files. **/
#include <stdio.h>                      /* Standard I/O operations. E.g. vprintf() */
#include <stdarg.h>                     /* Standard argument operations. E.g. va_list */
#include <sys/time.h>                   /* Time functions. E.g. gettimeofday() */
#include <atomic>                       /* Atomic operations. E.g. atomic<int> */
#include <chrono>                       /* Time functions. E.g. high_resolution_clock */
/** Defninitions. **/
#define MAX_FORMAT_LEN 255
#define DEBUG false
#define TITLE true
#define TIMER true
#define CUR  false

#define DEBUG_ON true
/** Classes. **/

class Debug
{
private:
    static long startTime;              /* Last start time in milliseconds. */
public:
    static std::chrono::high_resolution_clock::time_point start_time[32];
    static std::chrono::high_resolution_clock::time_point id_time[32];
    static double id[32];
    static std::atomic<int> timeCounter;
    static void debugTitle(const char *str); /* Print debug title string. */
    static void debugItem(const char *format, ...); /* Print debug item string. */
    static void debugCur(const char *format, ...); /* Print debug item string. */
    static void notifyInfo(const char *format, ...); /* Print normal notification. */
    static void notifyError(const char *format, ...); /* Print error information. */
    static void debugTimerStart(); /* Start debug timer. */
    static void debugTimerEnd(char *str = nullptr); /* End debug timer. */
    //the timer for loop  and every count will add to total
    static void debugTimerIDStart(int id); /* Start debug timer. */
    static void debugTimerIDEnd(int id); /* End debug timer. */
    static void debugTimeIDPrint(int id, char *str = nullptr);
    
};

/** Redundance check. **/
#endif
