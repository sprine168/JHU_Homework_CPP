#ifndef MODULE10_ELEVATOR_LOGGER_H
#define MODULE10_ELEVATOR_LOGGER_H


// Logger.h
#pragma once

#include <string>
#include <mutex>

// Boost.Log core
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/support/date_time.hpp> // date/time formatting
#include <boost/date_time/posix_time/posix_time.hpp>


class Logger {
public:

    ~Logger() = default;

    // call once at boot: Logger::init("boot.log");
    static void init(const std::string &logFile = "boot.log") {
        static std::once_flag once;
        std::call_once(once, [&]() {
            namespace logging = boost::log;
            namespace sinks = boost::log::sinks;
            namespace expr = boost::log::expressions;
            namespace keywords = boost::log::keywords;

            // add common attributes (TimeStamp, ProcessID, ThreadID, etc.)
            logging::add_common_attributes();

            // Set global filter (minimum severity)
            logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);


            // Console logging with ansi color
            logging::add_console_log(
                std::clog,
                keywords::format = expr::stream << std::fixed << std::setprecision(3) << "\x1b[30m"
                << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << "] [" << "] " << expr::smessage
            );

            // File sink with rotation
            logging::add_file_log(
                keywords::file_name = logFile,
                keywords::rotation_size = 10 * 1024 * 1024, // 10 MB
                keywords::open_mode = std::ios::app,
                keywords::auto_flush = true,
                keywords::format =
                expr::stream << std::fixed << std::setprecision(3)
                << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << "] [" << logging::trivial::severity
                << "] " << expr::smessage
            );
        });
    }
};


#endif //MODULE10_ELEVATOR_LOGGER_H
