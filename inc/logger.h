#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <jansson.h>
#include <string.h>
#include <unistd.h>


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
#define     TIME_COLOR      "\x1b[38:5:241m"

/* LOG STORAGE JSON KEY DEFINE */
#define     LOG_LIMIT       "log_limit"
#define     LOG_FILE        "log_file"
#define     LOG_START       "log_start_time"

/* LOG_CACHE PROFILE DEFINE */
#define     LOG_CACHE       ".cache"
#define     MQTT_LOG        "mqtt_log"

/* LOG LEVEL */
enum { DEBUG, INFO, ERROR, STORE };
enum { N_TIME, B_TIME };


static const char* log_level_color[] = { DEBUG_COLOR                 ,   INFO_COLOR              ,   ERROR_COLOR };
static const char* log_level_strings[] = { "[LOG_DEBUG]"               ,   "[LOG_INFO]"            ,   "[LOG_ERROR]" };
static const char* time_template[] = { "%Y-%m-%d %H:%M:%S"         ,   "[%Y-%m-%d %H:%M:%S]" };

#define     DEBUG(fmt   ,...)       logger_stdout( DEBUG   ,  __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)   
#define     INFO(fmt    ,...)       logger_stdout( INFO    ,  __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)
#define     ERROR(fmt   ,...)       logger_stdout( ERROR   ,  __FILE__,   __LINE__,   fmt"\n"   ,##__VA_ARGS__)

#define     STORE_DEBUG(fmt)        logger_storage( DEBUG       ,   fmt"\n")
#define     STORE_INFO(fmt)         logger_storage( INFO        ,   fmt"\n")
#define     STORE_ERROR(fmt)        logger_storage( ERROR       ,   fmt"\n")

#define     GET_N_TIME(buffer)              get_current_time( N_TIME    ,   buffer)
#define     GET_B_TIME(buffer)              get_current_time( B_TIME    ,   buffer)


void logger_stdout(int log_level, const char* file, int line, const char* fmt, ...);
void logger_storage(int log_level, const char* fmt);

int init_logger_file(char* log_name, long long log_limit);
void get_current_time(int type, char* buffer);
void read_cache(char* log_name, long long* log_limit);
int still_good_bro();
void delete_top_line(int number_lines);
#endif