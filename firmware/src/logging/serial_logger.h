#ifndef SERIAL_LOGGER_H
#define SERIAL_LOGGER_H (1)

#include <cstdint>
#include <cstdio>
#include <cstring>

#define MAX_MSG_LEN 256u
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

typedef enum
{
  LOG_DEBUG = 0,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  LOG_MC
} LogLevel;

void initSerialLogger(const LogLevel desired_logging_level);

void sendMessage(const LogLevel msg_log_level, const char* const msg);
#define logMessage(ll, format, ...)                                                      \
  {                                                                                      \
    char msg_buf[MAX_MSG_LEN];                                                           \
    const int used = snprintf(msg_buf, MAX_MSG_LEN, "[%s:%d] ", __FILENAME__, __LINE__); \
    if ((used > 0) && (used < (int)MAX_MSG_LEN))                                         \
    {                                                                                    \
      (void)snprintf(&msg_buf[used], (MAX_MSG_LEN - used), format, ##__VA_ARGS__);       \
    }                                                                                    \
    sendMessage(ll, msg_buf);                                                            \
  }

void threadSerialLogger(const void* argument);

#endif  // SERIAL_LOGGER_H
