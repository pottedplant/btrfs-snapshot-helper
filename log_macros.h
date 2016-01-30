#ifndef LOG_MACROS_H
#define LOG_MACROS_H

#include <sstream>

#include "log_core.h"

// LOG_S

#define LOG_MACROS_LOG_S_NO_LOG(level,category) if(false) std::stringstream()

#define LOG_MACROS_LOG_S(level,category) logging::stream_adapter(\
	logging::global::get(),\
	logging::info{\
		std::chrono::system_clock::now(),\
		logging::levels::level,\
		category,\
		logging::scopes::current()\
	}\
).stream

#ifdef LOG_LEVEL_NONE

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S_NO_LOG(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S_NO_LOG(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S_NO_LOG(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S_NO_LOG(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S_NO_LOG(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S_NO_LOG(fatal,category)

#elif LOG_LEVEL_FATAL

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S_NO_LOG(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S_NO_LOG(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S_NO_LOG(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S_NO_LOG(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S_NO_LOG(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S(fatal,category)

#elif LOG_LEVEL_ERROR

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S_NO_LOG(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S_NO_LOG(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S_NO_LOG(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S_NO_LOG(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S(fatal,category)

#elif LOG_LEVEL_WARN

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S_NO_LOG(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S_NO_LOG(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S_NO_LOG(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S(fatal,category)

#elif LOG_LEVEL_INFO

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S_NO_LOG(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S_NO_LOG(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S(fatal,category)

#elif LOG_LEVEL_DEBUG

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S_NO_LOG(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S(fatal,category)

#else

#define LOG_MACROS_LOG_S_trace(category) LOG_MACROS_LOG_S(trace,category)
#define LOG_MACROS_LOG_S_debug(category) LOG_MACROS_LOG_S(debug,category)
#define LOG_MACROS_LOG_S_info(category) LOG_MACROS_LOG_S(info,category)
#define LOG_MACROS_LOG_S_warn(category) LOG_MACROS_LOG_S(warn,category)
#define LOG_MACROS_LOG_S_error(category) LOG_MACROS_LOG_S(error,category)
#define LOG_MACROS_LOG_S_fatal(category) LOG_MACROS_LOG_S(fatal,category)

#endif

#define LOG_S(level,category) LOG_MACROS_LOG_S_##level(category)

// LOG_L

#define LOG_MACROS_LOG_L_NO_LOG(level,category,lambda) if(false);

#define LOG_MACROS_LOG_L(level,category,lambda) logging::global::get()->log(\
	logging::info{\
		std::chrono::system_clock::now(),\
		logging::levels::level,\
		category,\
		logging::scopes::current()\
	},\
	logging::lambda_message(lambda)\
);
#ifdef LOG_LEVEL_NONE

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L_NO_LOG(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L_NO_LOG(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L_NO_LOG(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L_NO_LOG(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L_NO_LOG(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L_NO_LOG(fatal,category,lambda)

#elif LOG_LEVEL_FATAL

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L_NO_LOG(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L_NO_LOG(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L_NO_LOG(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L_NO_LOG(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L_NO_LOG(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L(fatal,category,lambda)

#elif LOG_LEVEL_ERROR

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L_NO_LOG(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L_NO_LOG(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L_NO_LOG(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L_NO_LOG(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L(fatal,category,lambda)

#elif LOG_LEVEL_WARN

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L_NO_LOG(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L_NO_LOG(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L_NO_LOG(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L(fatal,category,lambda)

#elif LOG_LEVEL_INFO

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L_NO_LOG(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L_NO_LOG(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L(fatal,category,lambda)

#elif LOG_LEVEL_DEBUG

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L_NO_LOG(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L(fatal,category,lambda)

#else

#define LOG_MACROS_LOG_L_trace(category,lambda) LOG_MACROS_LOG_L(trace,category,lambda)
#define LOG_MACROS_LOG_L_debug(category,lambda) LOG_MACROS_LOG_L(debug,category,lambda)
#define LOG_MACROS_LOG_L_info(category,lambda) LOG_MACROS_LOG_L(info,category,lambda)
#define LOG_MACROS_LOG_L_warn(category,lambda) LOG_MACROS_LOG_L(warn,category,lambda)
#define LOG_MACROS_LOG_L_error(category,lambda) LOG_MACROS_LOG_L(error,category,lambda)
#define LOG_MACROS_LOG_L_fatal(category,lambda) LOG_MACROS_LOG_L(fatal,category,lambda)

#endif

#define LOG_L(level,category,lambda) LOG_MACROS_LOG_L_##level(category,lambda)

#endif
