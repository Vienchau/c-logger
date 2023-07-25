#include "logger.h"

int main()
{
    char time_buffer[30];
    GET_N_TIME(time_buffer);
    DEBUG("Start application at: %s", time_buffer);
    init_logger_file(MQTT_LOG, 1 * MB);
    DEBUG("Init cache done!");

    STORE_DEBUG("Start write to log!");
    STORE_INFO("store sample!");
    STORE_ERROR("i don't feel so good,...");
}