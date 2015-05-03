#pragma once

#include <iostream>
#include <sstream>

#define LOG_LEVEL_FATAL    " FATAL "//(0)
#define LOG_LEVEL_ERROR    " ERROR "//(1)
#define LOG_LEVEL_WARNING  "WARNING"//(2)
#define LOG_LEVEL_INFO     " INFO  "//(3)
#define LOG_LEVEL_DEBUG    " DEBUG "//(4)
#define LOG_LEVEL_TRACE    " TRACE "//(5)

#define LOG(LEVEL, CHAIN) {\
                std::stringstream ss;\
                ss << "[" << LEVEL << "] " << CHAIN << "|" << __FILE__ << " +" << __LINE__ << std::endl;\
                std::cout << ss.str() << std::flush;\
            }

#define LOG_FATAL(CHAIN)   LOG(LOG_LEVEL_FATAL, CHAIN)
#define LOG_ERROR(CHAIN)   LOG(LOG_LEVEL_ERROR, CHAIN)
#define LOG_WARNING(CHAIN) LOG(LOG_LEVEL_WARNING, CHAIN)
#define LOG_INFO(CHAIN)    LOG(LOG_LEVEL_INFO, CHAIN)
#ifdef DEBUG
#define LOG_DEBUG(CHAIN)   LOG(LOG_LEVEL_DEBUG, CHAIN)
#else
#define LOG_DEBUG(CHAIN)
#endif
#define LOG_TRACE(CHAIN)   LOG(LOG_LEVEL_TRACE, CHAIN)

