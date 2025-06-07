#include "silok/logger.hpp"

int main(int argc, char *argv[])
{
    SILOK_LOG_TRACE("TRACE LOG");
    SILOK_LOG_INFO("INFO LOG");
    SILOK_LOG_DEBUG("DEBUG LOG");
    SILOK_LOG_WARN("WARN LOG");
    SILOK_LOG_ERROR("ERROR LOG");
    SILOK_LOG_CRITICAL("CRITICAL LOG");
    return 0;
}