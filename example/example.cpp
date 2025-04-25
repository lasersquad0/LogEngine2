/*
 * Example.cpp
 *
 * Copyright 2025, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <cstdio>
#include <chrono>

#include "LogEngine.h"

void DefaultLoggerExample();
void FileLoggerExample();
void RotatingExample();
void LoadCFGExample();
void CallbackExample();
void AsyncExample();
//void StopwatchExample();
void MultiSinkExample();
//void replace_default_logger_example();

int main(int, char *[]) 
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtMemState s1, s2, s3;
    _CrtMemCheckpoint(&s1); // Take a snapshot at the start of main()

    try
    {
        LogEngine::Logger& logger = LogEngine::GetStdoutLogger("consolelogger");

        logger.InfoFmt("Welcome to LogEngine version {}.{}.{}  !",
            LOGENGINE_VER_MAJOR, LOGENGINE_VER_MINOR, LOGENGINE_VER_PATCH);

        logger.InfoFmt("Padding in numbers:  {:08d}", 12);
        logger.CritFmt("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
        logger.InfoFmt("Support for floats {:03.2f}", 1.23456);
        logger.InfoFmt("Positional args are {1} {0}..", "too", "supported");
        logger.InfoFmt("{:>8} aligned, {:<8} aligned", "right", "left");

        logger.InfoFmt("invalid messge ", "left", 7);

        // Runtime log levels

        // set log level for logger and for all its sinks 
        logger.SetLogLevel(LogEngine::Levels::llInfo);
        logger.Debug("This message should not be displayed!");
        logger.SetLogLevel(LogEngine::Levels::llTrace, false); // set log level for logger only
        logger.Debug("This message should not be displayed due to log level in sink!");
        // set log level for logger and for all its sinks
        logger.SetLogLevel(LogEngine::Levels::llTrace);
        logger.Debug("This message should be displayed..");

        logger.SetPattern("%LOGLEVE% %LOGLEVEL% %DATETIME% %LOGLEVEL% [%THREAD%] %MSG%");
        logger.Info("This an info message with custom log line format");
        logger.SetDefaultPattern(); // returning back default log line pattern
        logger.Info("This an info message with default log line format");

        DefaultLoggerExample();
        CallbackExample();
        FileLoggerExample();
        RotatingExample();
        LoadCFGExample();

        AsyncExample();
        //StopwatchExample();
        MultiSinkExample();
        //void replace_default_logger_example();

        // Drop all loggers in the registry.
        // This is optional (only mandatory if using windows + async log).
        LogEngine::ShutdownLoggers();
    }

    catch (const LogEngine::LogException &ex) 
    {
        std::printf("Log initialization failed: %s\n", ex.what());
        return 1;
    }

    _CrtMemCheckpoint(&s2); // Take a snapshot at the end of main()
    if (_CrtMemDifference(&s3, &s1, &s2)) _CrtMemDumpStatistics(&s3); // Dump memory statistics excluding global variables
}

// .lfg file location directory relative to current app directory
#define LFG_FILE_DIR "..\\..\\..\\example\\"

// folder for placing log files 
#define LOG_FILES_DIR LFG_FILE_DIR "logs\\"

void DefaultLoggerExample()
{
    // log level is llInfo by default
    LogEngine::Info("[Default logger] It is stdout logger.");
    LogEngine::Log("[Default logger] Warning message.", LogEngine::Levels::llWarning);
    LogEngine::Debug("[Default logger] Here is Debug message, will not be displayed");

    LogEngine::LogFmt(LogEngine::Levels::llError, "[Default logger] #1 This is formatted message {} {}", "arg1", 5);
    LogEngine::WarnFmt("[Default logger] #2 This is formatted message {} {}", 4*7, "argX");
}

void FileLoggerExample()
{
    // Create thread unsafe console logger
    auto& stdoutLogger = LogEngine::GetStdoutLoggerST("stdoutlogger");
    stdoutLogger.Info("Console stdout logger has created.");

    // GetStderrLogger is synonim of GetStderrLoggerST 
    auto& stderrLogger = LogEngine::GetStderrLogger("stderrlogger");
    stderrLogger.Info("Console stderr logger has created.");

    // Create thread safe file logger (not rotated).
    auto& fileLogger = LogEngine::GetFileLoggerMT("file_logger", LOG_FILES_DIR "basic_file_log.txt");
    fileLogger.Info("My file_logger has created.");
 
    //later you can get existing file logger using its name
    auto& fileLogger2 = LogEngine::GetLogger("file_logger");
    //fileLogger2 is the reference to the same file_logger created above
    fileLogger2.Info("My file_logger2 has got.");
}

//#include "RotatingFileSink.h"
void RotatingExample() 
{
    // Create a file rotating logger with 1kb size max and time stampts in file names.
    auto& rotLogger = LogEngine::GetRotatingFileLoggerST("rot_logger", LOG_FILES_DIR "rotating.txt", 
        1024, LogEngine::rsTimeStamp);
    
    // five rotating files will be generated during this loop
    // each file will be around 1Kb size, depending on size of the last message that 
    // exceeds the 1kb threshold
    for (int i = 1; i < 100; ++i)
    {
        rotLogger.InfoFmt("Rotating file message #{}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Create a file rotating logger with 1kb size max and 10 backup files.
    // This logger is thread safe - can be calles from different thtreads
    auto& rotLogger2 = LogEngine::GetRotatingFileLoggerMT("rot_logger_name", LOG_FILES_DIR "rotating2.txt", 
        1024, LogEngine::rsNumbers, 10);
    rotLogger2.Info("Rotating file message");
}

void CallbackExample() 
{
    auto& logger = LogEngine::GetCallbackLoggerST("custom_callback_logger",
        [](const LogEngine::LogEvent& /*le*/) 
        {
            std::cout << "Lambdadada " << std::endl;
            // do what you need to do with le
        });

    logger.Info("Origial message for callback logger");
}

void LoadCFGExample() 
{
    // load and construct loggers and sinks from .lfg file.
    // once loggers are initialised you can use GetLogger() to get needed logger by its name
    LogEngine::InitFromFile(LFG_FILE_DIR "LogExample.lfg");
    
    // this logger contains two sinks of File and Stdout types.
    // when we log into this logger messages will be sent into two targets.
    // logger name is case INSensitive here, so names below will considered as one logger: 
    // 'MainLogger', 'mainlogger', 'MAINLOGGER'
    auto& mainlogger = LogEngine::GetLogger("MainLogger");
    mainlogger.Error("This message will be sent into two targets: file and console (stdout).");
    mainlogger.Info("This message will be sent into file only since console sink has Loglevel=Error");

    // this logger contains two sinks of File and Stdout types.
    // stdout with name 's2' sink is shared with Mainlogger above
    auto& second = LogEngine::GetLogger("SECOND");
    second.Error("This message will be sent into three targets: file, console and rotating file.");
    second.Warn("This message will be sent into two targets file and rotating file");
}

void AsyncExample() 
{
    // in Async mode all log messages are added into queue in memory and written to 
    // file (or sent to other targets) by a separate thread. 
    // it means that main thread does not wait until log messages written to the file
    // that minimizes effect from log calls
    auto& async_logger = LogEngine::GetFileLoggerST("async_logger", LOG_FILES_DIR "async_log.txt");
    async_logger.SetAsyncMode(true);

    for (int i = 1; i < 101; ++i) 
    {
        async_logger.InfoFmt("Async message #{}", i);
    }
}

// Two loggers with multiple shared sinks, each with a different format and log level.
void MultiSinkExample()
{
    using namespace LogEngine;
    
    // we need shared_ptr for Sinks here for proper freeing Sink objects when they shared between several loggers. 
    std::shared_ptr<Sink> consoleSink(new StdoutSinkST("console_sink"));
    consoleSink->SetLogLevel(Levels::llWarning);
    consoleSink->SetPattern("[multi_sink_example] [%loglevel% %TIME% #%thread%] %Msg%");

    std::shared_ptr<Sink> fileSink(new FileSinkST("file_sink", LOG_FILES_DIR "multisink.txt"));
    fileSink->SetLogLevel(Levels::llTrace);

    Logger logger("multi_sink", { consoleSink, fileSink });
    logger.SetLogLevel(Levels::llDebug, false); // change log level for logger only.
    logger.Warn("This should appear in both console and file");
    logger.Info("This message should not appear in the console, only in the file");

    // two sinks are shared between two loggers
    // logger2 contains two sinks because duplicates
    Logger logger2("multi_sink2", { consoleSink, fileSink, consoleSink, fileSink });
    logger2.SetLogLevel(Levels::llDebug, false); // change log level for logger only.
    logger2.Warn("This should appear two times in both console and file");
    logger2.Info("This message should not appear in the console, only in the file two times");
}

/*
// stopwatch example
void StopwatchExample()
{
    stopwatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(999));

    auto& stopLogger = LogEngine::GetFileLogger("loggerforstopwatch", LOG_FILES_DIR "stopwatch_log.txt");
    stopLogger.InfoFmt("Stopwatch: {} seconds", sw);
}

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
void replace_default_logger_example() 
{
    // store the old logger so we don't break other examples.
    auto old_logger = spdlog::default_logger();

    auto new_logger = spdlog::basic_logger_mt("new_default_logger", "logs/new-default-log.txt", true);
    spdlog::set_default_logger(new_logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::debug("This message should not be displayed!");
    spdlog::set_level(spdlog::level::trace);
    spdlog::debug("This message should be displayed..");

    spdlog::set_default_logger(old_logger);
}
*/