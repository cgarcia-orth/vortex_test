/**************************************************************************************************
* Module Name    : LoggerFactory.h
*
* Description    : Utility functions to build loggers.
*
* Copyright (c) 2019 Vortex Aquatic Structures International Inc. All Rights Reserved.
*
* Revision history:
*
*  2019-07-30  Olivier Allaire : Initial implementation.
*
**************************************************************************************************/


#ifndef LOGGER_LOGGERFACTORYFACTORY_H_
#define LOGGER_LOGGERFACTORYFACTORY_H_

#include <vector>

#include <spdlog/spdlog.h>

namespace vortex {

class LoggerFactory {
public:
	static void init(const spdlog::filename_t& filename, spdlog::level::level_enum loglevel);
    static void initTest();
	static std::shared_ptr<spdlog::logger> getLogger(const std::string& name, bool registerLogger = false);
    static void dropLogger(const std::shared_ptr<spdlog::logger>& logger);
private:
	static spdlog::level::level_enum s_level;
	static const std::string s_format;
	static std::vector<spdlog::sink_ptr> s_sinks;
};

} /* namespace vortex */

#endif /* LOGGER_LOGGERFACTORYFACTORY_H_ */
