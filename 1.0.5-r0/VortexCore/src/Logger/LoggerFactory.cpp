/**************************************************************************************************
 * Module Name    : LoggerFactory.cpp
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

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "Logger/LoggerFactory.h"

namespace vortex {

spdlog::level::level_enum LoggerFactory::s_level = spdlog::level::info;
const std::string LoggerFactory::s_format = "[%l][%D %H:%M:%S:%e][%n]: %v";
std::vector<spdlog::sink_ptr> LoggerFactory::s_sinks;


void LoggerFactory::init(const spdlog::filename_t& filename, spdlog::level::level_enum loglevel) {
    LoggerFactory::s_level = loglevel;
    spdlog::set_level(loglevel);
	spdlog::init_thread_pool(8192, 1);
	// Log to console
	auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
	stdout_sink->set_pattern(LoggerFactory::s_format);
	stdout_sink->set_level(LoggerFactory::s_level);
	LoggerFactory::s_sinks.push_back(stdout_sink);
	// Log to rotating file
	auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			filename, 1024 * 1024 * 10, 3);
	rotating_sink->set_pattern(LoggerFactory::s_format);
	rotating_sink->set_level(LoggerFactory::s_level);
	LoggerFactory::s_sinks.push_back(rotating_sink);
}

std::shared_ptr<spdlog::logger> LoggerFactory::getLogger(const std::string& name, bool registerLogger) {
	auto logger = std::make_shared<spdlog::async_logger>(name,
			LoggerFactory::s_sinks.begin(), LoggerFactory::s_sinks.end(),
			spdlog::thread_pool(), spdlog::async_overflow_policy::block);
	if (registerLogger) {
        spdlog::register_logger(logger);
    }
	logger->set_level(LoggerFactory::s_level);
    (*logger).flush_on(LoggerFactory::s_level);
	return logger;
}

void LoggerFactory::dropLogger(const std::shared_ptr<spdlog::logger>& logger) {
    spdlog::drop(logger->name());
}


void LoggerFactory::initTest() {
    LoggerFactory::s_level = spdlog::level::debug;
    spdlog::set_level(LoggerFactory::s_level);
    spdlog::init_thread_pool(8192, 1);
    // Log to console
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    stdout_sink->set_pattern(LoggerFactory::s_format);
    stdout_sink->set_level(spdlog::level::debug);
    LoggerFactory::s_sinks.push_back(stdout_sink);
}

} /* namespace vortex */
