#include "logger.h"

int is_file_exist(char* file_name);

void logger_stdout(int log_level, const char* file, int line, const char* fmt, ...)
{
    char time_buffer[30];
    va_list args;
    va_start(args, fmt);
    FILE* buffer = stderr;
    /* Get time for logging*/
#ifdef USE_TIMESTAMP
    GET_B_TIME(time_buffer);
#else
    sprintf(time_buffer, "");
#endif

    char file_buffer[256];
#ifdef USE_DEBUG_LINE
    sprintf(file_buffer, "[%s:%d]", file, line);
#else
    sprintf(file_buffer, "");
#endif

#ifdef USE_COLOR
    fprintf(buffer, "%s%s%s %s%s%s %s%s%s: ", TIME_COLOR,
        time_buffer,
        RESET_COLOR,
        log_level_color[log_level],
        log_level_strings[log_level],
        RESET_COLOR,
        TIME_COLOR,
        file_buffer,
        RESET_COLOR);
#else
    fprintf(buffer, "%s %s %s: ", time_buffer,
        log_level_strings[log_level],
        file_buffer);
#endif
    vfprintf(buffer, fmt, args);
    va_end(args);
}

void logger_storage(int log_level,  const char* fmt,  log_profile_t* log_profile){
    if (!still_good_bro(log_profile))
    {
        delete_top_line(log_profile->log_name, NUMBER2DEL);
    }

    FILE* buffer = fopen(log_profile->log_name, "a+");
    char time_buffer[40];
    GET_B_TIME(time_buffer);

    fprintf(buffer, "%s %s: %s\n", time_buffer, log_level_strings[log_level], fmt);
    fclose(buffer);
}

void get_current_time(int type, char* buffer)
{
    char* time_buffer_ptr = buffer;
    char temp[30];
    time_t t = time(NULL);
    struct tm* time = localtime(&t);
    size_t len = strftime(temp, sizeof(temp), time_template[type], time);
    temp[len] = '\0';
    strncpy(time_buffer_ptr, temp, strlen(temp) + 1);
}

log_profile_t* init_logger_file(char* log_name, long long log_limit)
{
    if(!is_file_exist(log_name))
    {
        FILE *fp = fopen(log_name, "w");
        fclose(fp);
    }
    log_profile_t* log_profile = (log_profile_t*)malloc(sizeof(log_profile_t));
    strncpy(log_profile->log_name, log_name, strlen(log_name) + 1);
    log_profile->log_limit = log_limit;
    return log_profile;
}

void close_log_profile(log_profile_t* log_profile)
{
    if(log_profile)
    {
        free(log_profile);
    }
}

int still_good_bro(log_profile_t* log_profile)
{
    FILE* log_file = fopen(log_profile->log_name, "r");
    fseek(log_file, 0L, SEEK_END);
    long sz = ftell(log_file);
    if (sz > log_profile->log_limit)
    {
        fclose(log_file);
        return FALSE;
    }
    rewind(log_file);
    fclose(log_file);
    return TRUE;
}

void delete_top_line(char* log_name, int number_lines)
{
    char log_temp[100];
    strncpy(log_temp, "temp", strlen("temp") + 1);

    FILE* fptr1, * fptr2;

    fptr1 = fopen(log_name, "r");
    if (!fptr1)
    {
        ERROR("can't open old log, deleted top lines fail!");
        exit(1);
    }
    fptr2 = fopen(log_temp, "w");
    if (!fptr2)
    {
        ERROR("can't open new log, deleted top lines fail!");
        exit(1);
    }

    int line_counter = 0;
    char buffer[1024];

    while (!feof(fptr1))
    {
        fgets(buffer, 1024, fptr1);
        if ((line_counter > number_lines) && (!feof(fptr1)))
        {
            fprintf(fptr2, "%s", buffer);
        }
        line_counter++;
    }
    fclose(fptr1);
    fclose(fptr2);

    if (!remove(log_name) == 0)
    {
        exit(-1);
    }

    rename(log_temp, log_name);
}


int is_file_exist(char* file_name)
{
    if (access(file_name, F_OK) == 0) {
        return TRUE;
    }
    return FALSE;
}
