#pragma once

/*
Before including this file you must define WL_FILENAME to be the name of your file (e.g. #define WL_FILENAME "main.cpp")
if you are planning to use WL_LOG_ERRORF, WL_LOG_WARNINGF, WL_LOG_DEBUGF, WL_LOG_INFOF.
*/

#include <iostream>

// Toggle these flags on/off to filter different types of debug messages
#define WL_LOGGER_ENABLED_ERROR 1
#define WL_LOGGER_ENABLED_WARNING 1
#define WL_LOGGER_ENABLED_INFO 1
#define WL_LOGGER_ENABLED_DEBUG 1

// WL_LOG_ERROR logs an error message to the console
// WL_LOG_ERRORG logs an error message to the console together with the filename of the file where it's called from
#if WL_LOGGER_ENABLED_ERROR
#define WL_LOG_ERROR(MSG) std::cout << "[ERROR]: " << MSG << std::endl;
#ifdef WL_FILENAME
#define WL_LOG_ERRORF(MSG) std::cout << "[ERROR in " << WL_FILENAME << "]: " << MSG << std::endl;
#else // WL_FILENAME
#define WL_LOG_ERRORF(MSG) You must define WL_FILENAME to be the name of your file (e.g. #define WL_FILENAME "main.cpp")
#endif // WL_FILENAME
#else // WL_LOGGER_ENABLED_ERROR
#define WL_LOG_ERROR(MSG)
#define WL_LOG_ERRORF(MSG)
#endif // WL_LOGGER_ENABLED_ERROR

// WL_LOG_WARNING logs a warning message to the console
// WL_LOG_WARNINGF logs a warning message to the console together with the filename of the file where it's called from
#if WL_LOGGER_ENABLED_WARNING
#define WL_LOG_WARNING(MSG) std::cout << "[WARNING]: " << MSG << std::endl;
#ifdef WL_FILENAME
#define WL_LOG_WARNINGF(MSG) std::cout << "[WARNING in " << WL_FILENAME << "]: " << MSG << std::endl;
#else // WL_FILENAME
#define WL_LOG_WARNINGF(MSG) You must define WL_FILENAME to be the name of your file (e.g. #define WL_FILENAME "main.cpp")
#endif // WL_FILENAME
#else // WL_LOGGER_ENABLED_WARNING
#define WL_LOG_WARNING(MSG)
#define WL_LOG_WARNINGF(MSG)
#endif // WL_LOGGER_ENABLED_WARNING

// WL_LOG_INFO logs an info message to the console
// WL_LOG_INFOF logs an info message to the console together with the filename of the file where it's called from
#if WL_LOGGER_ENABLED_INFO
#define WL_LOG_INFO(MSG) std::cout << "[INFO]: " << MSG << std::endl;
#ifdef WL_FILENAME
#define WL_LOG_INFOF(MSG) std::cout << "[INFO in " << WL_FILENAME << "]: " << MSG << std::endl;
#else // WL_FILENAME
#define WL_LOG_INFOF(MSG) You must define WL_FILENAME to be the name of your file (e.g. #define WL_FILENAME "main.cpp")
#endif // WL_FILENAME
#else // WL_LOGGER_ENABLED_INFO
#define WL_LOG_INFO(MSG)
#define WL_LOG_INFOF(MSG)
#endif // WL_LOGGER_ENABLED_INFO

// WL_LOG_DEBUG logs a debug message to the console
// WL_LOG_DEBUGF logs a debug message to the console together with the filename of the file where it's called from
#if WL_LOGGER_ENABLED_DEBUG
#define WL_LOG_DEBUG(MSG) std::cout << "[DEBUG]: " << MSG << std::endl;
#ifdef WL_FILENAME
#define WL_LOG_DEBUGF(MSG) std::cout << "[DEBUG in " << WL_FILENAME << "]: " << MSG << std::endl;
#else // WL_FILENAME
#define WL_LOG_DEBUGF(MSG) You must define WL_FILENAME to be the name of your file (e.g. #define WL_FILENAME "main.cpp")
#endif // WL_FILENAME
#else // WL_LOGGER_ENABLED_DEBUG
#define WL_LOG_DEBUG(MSG)
#define WL_LOG_DEBUGF(MSG)
#endif // WL_LOGGER_ENABLED_DEBUG
