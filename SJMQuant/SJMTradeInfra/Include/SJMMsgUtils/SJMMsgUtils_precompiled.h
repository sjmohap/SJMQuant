#pragma once

#include <stdint.h>
#include <mutex>
#include <string>
#include <nanomsg\nn.h>
#include <nanomsg\pair.h>
#include <nanomsg\pubsub.h>
#include <assert.h>
#include <zmq.h>

#include <SJMLogger/SJMLogger.h>


#define LOG_PRINT(...) do{\
SJMLogger::instance().PrintToFile(__VA_ARGS__);\
SJMLogger::instance().displayInConsole(__VA_ARGS__);\
}while (0)

#define LOG(...) do{\
SJMLogger::instance().PrintToFile(__VA_ARGS__);\
}while (0)

#define PRINT(...) do{\
SJMLogger::instance().displayInConsole(__VA_ARGS__);\
}while (0)