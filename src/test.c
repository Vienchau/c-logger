#include "logger.h"

int main()
{
    log_profile_t* log_profile = init_logger_file("mqtt_log", 1 * MB);
    STORE_DEBUG("this is debug message!",log_profile);
    STORE_INFO("this is info message!",log_profile);
    STORE_ERROR("this is error message!",log_profile);
    STORE_TRACE("this is trace message!",log_profile);
    close_log_profile(log_profile);

    DEBUG("This is debug info with color: %s", "blue");
    INFO("This is info info with color: %s", "green");
    ERROR("This is error info with color: %s", "red");
    TRACE("This is error info with color: %s", "orange");
}