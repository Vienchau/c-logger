#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* USAGE DEFINE */
#define     TRUE            1
#define     FALSE           0
#define     MB              1048576
#define     KB              1024
#define     NUMBER2DEL      300


/* COLOR DEFINE */
#define     RESET_COLOR     "\033[0m"
#define     INFO_COLOR      "\x1b[38;5;35m"
#define     DEBUG_COLOR     "\x1b[38;5;75m"
#define     ERROR_COLOR     "\x1b[31m"
#define     TRACE_COLOR     "\x1b[38:5:202m"
#define     TIME_COLOR      "\x1b[38:5:241m"

/* LOG STORAGE JSON KEY DEFINE */
#define     LOG_LIMIT       "log_limit"
#define     LOG_FILE        "log_file"
#define     LOG_START       "log_start_time"


/* LOG LEVEL */
enum { DEBUG, INFO, ERROR, TRACE, MAX_LEVEL };
enum { N_TIME, B_TIME };

#ifndef __AK_PACKETED
#define __AK_PACKETED __attribute__((__packed__))
#endif

/* LOG INFO STRUCT DEFINE*/
typedef struct
{
    char    log_name[100]   ;
    long    long log_limit  ;
}__AK_PACKETED log_profile_t;



static const char* log_level_color[MAX_LEVEL]    =   { DEBUG_COLOR                 ,   INFO_COLOR              ,   ERROR_COLOR   ,   TRACE_COLOR         };
static const char* log_level_strings[MAX_LEVEL]  =   { "[LOG_DEBUG]"               ,   "[LOG_INFO]"            ,   "[LOG_ERROR]" ,   "[RUNTIME_TRACE]"   };
static const char* time_template[]      =   { "%Y-%m-%d %H:%M:%S"         ,   "[%Y-%m-%d %H:%M:%S]"                     };

#define     DEBUG(fmt   ,...)   logger_stdout( DEBUG    ,   __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)   
#define     INFO(fmt    ,...)   logger_stdout( INFO     ,   __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)
#define     ERROR(fmt   ,...)   logger_stdout( ERROR    ,   __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)
#define     TRACE(fmt   ,...)   logger_stdout( TRACE    ,   __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)

#define     STORE_DEBUG(fmt     ,   log_profile_t    )       logger_storage( DEBUG   ,   fmt,    log_profile_t)
#define     STORE_INFO(fmt      ,   log_profile_t    )       logger_storage( INFO    ,   fmt,    log_profile_t)
#define     STORE_ERROR(fmt     ,   log_profile_t    )       logger_storage( ERROR   ,   fmt,    log_profile_t)
#define     STORE_TRACE(fmt     ,   log_profile_t    )       logger_storage( TRACE   ,   fmt,    log_profile_t)

#define     GET_N_TIME(buffer)  get_current_time( N_TIME    ,   buffer)
#define     GET_B_TIME(buffer)  get_current_time( B_TIME    ,   buffer)



void logger_stdout(int log_level, const char* file, int line, const char* fmt, ...);
void logger_storage(int log_level,  const char* fmt,  log_profile_t* log_profile);



extern log_profile_t* init_logger_file(char* log_name, long long log_limit);
extern void close_log_profile(log_profile_t* log_profile);
static void get_current_time(int type, char* buffer);
static int still_good_bro(log_profile_t* log_profile);
static void delete_top_line(char* log_name, int number_lines);

#endif