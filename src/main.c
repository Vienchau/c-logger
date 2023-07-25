#include "logger.h"

int main()
{
    log_profile_t* log_profile = init_logger_file("mqtt_log", 1 * MB);
    log_profile_t* log_profle_2 = init_logger_file("mqtt_log_2", 1*MB);
    for(int i = 0; i <  100000; i++)
    {
        STORE_DEBUG("new log!", log_profile);
        STORE_DEBUG("new log 2!", log_profle_2);
    }
    close_log_profile(log_profile);
    close_log_profile(log_profle_2);
}