#pragma once

#include <stdio.h>

enum LogLevel {
    L_DEBUG = 0,
    L_INFO  = 1,
    L_WARN = 2,
    L_ERROR = 3,
    L_FATAL = 4
};

extern int g_log_level;

// color  print
// #define LOG_DEBUG(format, ...)  {if (L_DEBUG >= g_log_level)  printf("\e[1;30m[debug] " format "\e[0m\n", ##__VA_ARGS__); }
// #define LOG_INFO(format, ...)   {if (L_INFO >= g_log_level)   printf("\e[1;37m[info ] " format "\e[0m\n", ##__VA_ARGS__);}
// #define LOG_WARN(format, ...)   {if (L_WARN >= g_log_level)   printf("\e[1;33m[warn ] " format "\e[0m\n", ##__VA_ARGS__);}
// #define LOG_ERROR(format, ...)  {if (L_ERROR >= g_log_level)  printf("\e[1;31m[error] " format "\e[0m\n", ##__VA_ARGS__);}
// #define LOG_FATAL(format, ...)  {if (L_FATAL >= g_log_level)  printf("\e[1;31m[fatal] " format "\e[0m\n", ##__VA_ARGS__);}

// no endl
#define LOG_DEBUG(format, ...)  {if (L_DEBUG >= g_log_level)  printf("\e[1;30m[debug] " format "\e[0m", ##__VA_ARGS__); }
#define LOG_INFO(format, ...)   {if (L_INFO >= g_log_level)   printf("\e[1;37m[info ] " format "\e[0m", ##__VA_ARGS__);}
#define LOG_WARN(format, ...)   {if (L_WARN >= g_log_level)   printf("\e[1;33m[warn ] " format "\e[0m", ##__VA_ARGS__);}
#define LOG_ERROR(format, ...)  {if (L_ERROR >= g_log_level)  printf("\e[1;31m[error] " format "\e[0m", ##__VA_ARGS__);}
#define LOG_FATAL(format, ...)  {if (L_FATAL >= g_log_level)  printf("\e[1;31m[fatal] " format "\e[0m", ##__VA_ARGS__);}