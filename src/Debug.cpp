/*** Debug source. ***/

/** Version 1 + Functional Model Modification **/

/** Included files. **/
#include "Debug.h"

/** Implemented functions. **/
/* Print debug title string.
   @param   str     String of debug title. */
void Debug::debugTitle(const char *str) {
    if (TITLE == true)                          /* If debug option is set. */
        printf("\033[0;45;1m%s\033[0m\n", str); /* Print debug title string. */
}

/* Print debug item string. Can be used in a formatted style like a printf().
   @param   format  Format of debug item. Same as printf().
                    POTENTIALPROBLEM: the length of format can not exceed
   MAX_FORMAT_LEN - 1, but there is no check.
   @param   ...     Other argument variables to print. Same as printf(). */
void Debug::debugItem(const char *format, ...) {
    char newFormat[MAX_FORMAT_LEN];

    va_list args;
    va_start(args, format); /* Start of variable arguments. */

    if (DEBUG_ON == true) /* If debug option is set. */
    {
        sprintf(newFormat, "\033[0;42;1m%s\033[0m\n",
                format);          /* Wrap format in a style. */
        vprintf(newFormat, args); /* Print string of debug item. */
    }

    va_end(args); /* End of variable arguments. */
}

void Debug::debugCur(const char *format, ...) {
    char newFormat[MAX_FORMAT_LEN];

    va_list args;
    va_start(args, format); /* Start of variable arguments. */

    if (CUR == true) /* If debug option is set. */
    {
        sprintf(newFormat, "%s\n", format); /* Wrap format in a style. */
        vprintf(newFormat, args);           /* Print string of debug item. */
    }

    va_end(args); /* End of variable arguments. */
}
/* Print necessary information at start period. Can be used in a formatted style
   like a printf().
   @param   format  Format of debug item. Same as printf().
                    POTENTIALPROBLEM: the length of format can not exceed
   MAX_FORMAT_LEN - 1, but there is no check.
   @param   ...     Other argument variables to print. Same as printf(). */
void Debug::notifyInfo(const char *format, ...) {
    char newFormat[MAX_FORMAT_LEN];

    va_list args;
    va_start(args, format); /* Start of variable arguments. */
    sprintf(newFormat, "\033[4m%s\033[0m\n",
            format);          /* Wrap format in a style. */
    vprintf(newFormat, args); /* Print string of notify information. */
    va_end(args);             /* End of variable arguments. */
}

/* Print error information at start period. Can be used in a formatted style
   like a printf().
   @param   format  Format of debug item. Same as printf().
                    POTENTIALPROBLEM: the length of format can not exceed
   MAX_FORMAT_LEN - 1, but there is no check.
   @param   ...     Other argument variables to print. Same as printf(). */
void Debug::notifyError(const char *format, ...) {
    char newFormat[MAX_FORMAT_LEN];

    va_list args;
    va_start(args, format); /* Start of variable arguments. */
    sprintf(newFormat, "\033[0;31m%s\033[0m\n",
            format);          /* Wrap format in a style. */
    vprintf(newFormat, args); /* Print string of notify information. */
    va_end(args);             /* End of variable arguments. */
}
std::chrono::high_resolution_clock::time_point Debug::start_time[32];
std::chrono::high_resolution_clock::time_point Debug::id_time[32];
std::atomic<int> Debug::timeCounter(0);
double Debug::id[32] = {0};
void Debug::debugTimerStart() {
    if (TIMER == true) {
        start_time[timeCounter % 32] = std::chrono::high_resolution_clock::now();
        timeCounter++;
    }
}

void Debug::debugTimerEnd(char *str ) {
    if (TIMER == true) {
        std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time[(timeCounter - 1)%32]);
        printf("Timer %s: %lf seconds.\n", str, time_span.count());
    }
}
void Debug::debugTimerIDStart(int idx){
    id_time[idx] = std::chrono::high_resolution_clock::now();
}
void Debug::debugTimerIDEnd(int idx){
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - id_time[idx]);
    id[idx] += time_span.count();
}
void Debug::debugTimeIDPrint(int idx, char *str){
    printf("Timer %d or %s : %lf seconds.\n", idx, str, id[idx]);
    id[idx] = 0;
}




