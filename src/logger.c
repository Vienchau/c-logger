#include "logger.h"

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

void logger_storage(int log_level, const char* fmt)
{
    long long log_limit;
    char log_name[100];
    read_cache(log_name, &log_limit);

    if (!still_good_bro())
    {
        delete_top_line(NUMBER2DEL);
    }

    FILE* buffer = fopen(log_name, "a+");
    char time_buffer[40];
    GET_B_TIME(time_buffer);

    fprintf(buffer, "%s %s: %s", time_buffer, log_level_strings[log_level], fmt);
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

int init_logger_file(char* log_name, long long log_limit)
{
    if (access(LOG_CACHE, F_OK) == 0) {
        if (remove(LOG_CACHE) == 0)
        {
            DEBUG("log cache exist, remove successfully.");
        }
        else
        {
            DEBUG("log cache exist, remove failed. Exit...");
            exit(-1);
        }
    }
    char time[30];
    GET_N_TIME(time);
    json_t* root = json_object();
    json_object_set_new(root, LOG_START, json_string(time));
    json_object_set_new(root, LOG_FILE, json_string(log_name));
    json_object_set_new(root, LOG_LIMIT, json_integer(log_limit));

    FILE* log_cache, * log_file;
    log_cache = fopen(LOG_CACHE, "w");
    json_dumpf(root, log_cache, JSON_COMPACT);

    json_decref(root);
    fclose(log_cache);
}

void read_cache(char* log_name, long long* log_limit)
{
    json_error_t error;

    FILE* log_cache = fopen(LOG_CACHE, "r");
    json_t* root = json_loadf(log_cache, 0, &error);
    char* temp = (char*)json_string_value(json_object_get(root, LOG_FILE));
    strncpy(log_name, temp, strlen(temp) + 1);
    *log_limit = json_integer_value(json_object_get(root, LOG_LIMIT));

    fclose(log_cache);
    json_decref(root);
}

int still_good_bro()
{
    char log_name[100];
    long long log_limit;
    read_cache(log_name, &log_limit);

    FILE* log_file = fopen(log_name, "r");
    fseek(log_file, 0L, SEEK_END);
    long sz = ftell(log_file);
    if (sz > log_limit)
    {
        return FALSE;
    }
    return TRUE;
}

void delete_top_line(int number_lines)
{
    char log_name[100], log_temp[100];
    long long log_limit;
    read_cache(log_name, &log_limit);

    strncpy(log_temp, "temp", strlen("temp") + 1);

    FILE* fptr1, * fptr2;

    fptr1 = fopen(log_name, "r");
    if (!fptr1)
    {
        STORE_ERROR("can't open old log, deleted top lines fail!");
        exit(1);
    }
    fptr2 = fopen(log_temp, "w");
    if (!fptr2)
    {
        STORE_ERROR("can't open new log, deleted top lines fail!");
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