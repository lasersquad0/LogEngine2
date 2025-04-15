# LogEngine2

LogEngine2 C++ logging library

## Overview
LogEngine2 is the C++ library for easy and flexible logging to files.
This is second and completely redesigned from scratch version of old LogEngine library.

The main advantages are: simple, easy to use, platform independent, small amount of source code.

Bug reports are welcome. Please use the Github bug tracking system at https://github.com/lasersquad0/LogEngine2. 

See testXXX.cpp files from test/ directory as an examples of using logengine library.
See test/testFiles/example.lfg - for full description of logengine parameters file.

Email to the current maintainer at lasersquad@gmail.com.

## Install
#### Microsoft Windows
Use MSVC project from folder prj/msvc/LogEngine/ to build LogEngine.lib (or LogEngined.lib for Debug configuration).

See prj/msvc/testLogEngine (or prj/Builder6/testLogEngine) for examples how to use LogEngine2.

#### Other systems
For other systems see INSTALL txt file for details.

## Platforms
* Windows (Visual Studio 2022)
* Linux and similar

## Features
* [][][][]Very fast (see [benchmarks](#benchmarks) below).
* [][][][]Headers only or compiled
* Feature-rich formatting, using the std::format available in C++20.
* Load loggers configuration from .lfg file
* Asynchronous mode (can be turned on and off)
* [][][]Multi/Single threaded loggers.
* Various log targets:
  * Regular log files
  * Rotating log files.
  * Console logging (stdio and stderr) [][][](colors supported).
  * String stream
  * [][]syslog.
  * [][]Windows event log.
  * [][]Windows debugger (```OutputDebugString(..)```).
  * Easily [extendable](https://github.com/gabime/spdlog/wiki/4.-Sinks#implementing-your-own-sink) with custom log targets.
* Configurable log line patterns which may include info: 
 * Date and/or Time 
 * Thread ID, 
 * Log level mark or full/short level name 
 * Application name and app version
 * OS name and OS version
* Log filtering - log levels can be modified at runtime as well as compile time.
* Log statistics
* Includes convenient Dynamic Arrays classes that can be used separately from LogEngine2 library.
* [][][]Support for loading log levels from argv or environment var.

## Usage samples

#### Basic usage
```c++
#include "LogEngine.h"

int main() 
{
   LogEngine::Logger& logger = LogEngine::GetStdoutLogger("consolelogger");
   
   logger.InfoFmt("Welcome to LogEngine version {}.{}.{}  !", LOGENGINE_VER_MAJOR, LOGENGINE_VER_MINOR, LOGENGINE_VER_PATCH);
   
   logger.InfoFmt("Padding in numbers:  {:08d}", 12);
   logger.CritFmt("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
   logger.InfoFmt("Support for floats {:03.2f}", 1.23456);
   logger.InfoFmt("Positional args are {1} {0}..", "too", "supported");
   logger.InfoFmt("{:>8} aligned, {:<8} aligned", "right", "left");

   // Runtime log levels
   logger.SetLogLevel(LogEngine::Levels::llInfo);  // set log level for all logger sinks to llInfo
   logger.Debug("This message should not be displayed!");
   logger.SetLogLevel(LogEngine::Levels::llTrace);  // set specific logger's log level (for all sinks)
   logger.Debug("This message should be displayed..");

   logger.SetPattern("%LOGLEVE% %LOGLEVEL% %DATETIME% %LOGLEVEL% [%THREAD%] %MSG%");
   logger.Info("This an info message with custom log line format");
   logger.SetDefaultPattern(); // returning back default log line pattern
   logger.Info("This an info message with default log line format");
   
}

```
---
#### Create stdout/stderr and file logger objects
```c++
void stdout_example()
{
    auto& stdout_logger = LogEngine::GetStdoutLogger("stdoutlogger");
    stdout_logger.Info("Console stdout logger has created.");

    auto& stderr_logger = LogEngine::GetStderrLogger("stderrlogger");
    stderr_logger.Info("Console stderr logger has created.");

    // Create file logger (not rotated). 
    auto& file_logger = LogEngine::GetFileLogger("file_logger", LOG_FILES_DIR "basic_file_log.txt");
    file_logger.Info("My file_logger has created.");
 
    //later you can get existing file logger using its name
    auto& file_logger2 = LogEngine::GetLogger("file_logger");
    //file_logger2 is the reference to the same file_logger created above
    file_logger2.Info("My file_logger2 has got."); 
}
```

---
#### Rotating files
```c++
    // Create a file rotating logger with 1kb size max and 10 rotated files.
    LogEngine::Logger& rotLogger = LogEngine::GetRotatingFileLogger("some_logger_name", LOG_FILES_DIR "rotating.txt", 1024,     LogEngine::rsTimeStamp, 10);

    // five rotating files will be generated during this for loop
    // each file will be around 1Kb size, depending on size of the last message that exceeds the 1kb threshold
    for (int i = 1; i < 100; ++i)
    {
        rotLogger.InfoFmt("Rotating file message #{}", i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }```

---
#### Stopwatch
```c++
// Stopwatch support for spdlog
#include "spdlog/stopwatch.h"
void stopwatch_example()
{
    spdlog::stopwatch sw;    
    spdlog::debug("Elapsed {}", sw);
    spdlog::debug("Elapsed {:.3}", sw);       
}

```

---
#### Logger with multi sinks - each with a different format and log level
```c++

// create a logger with 2 targets, with different log levels and formats.
// The console will show only warnings or errors, while the file will log all.
void multi_sink_example()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_level(spdlog::level::debug);
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");
}
```

---
#### User-defined logger config file (*.lfg)
```c++

// loads and constructs loggers and sinks from .lfg file.
// once loggers are initialized you can use GetLogger() function to get needed logger by its name
void callback_example()
{
    LogEngine::InitFromFile(LFG_FILE_DIR "LogExample.lfg");

    // this logger contains two sinks of File and Stdout types.
    // when we log into this logger messages will be sent into two targets.
    // logger name is case INsensitive here, so these names are considered as names of one logger: 'MainLogger', 'mainlogger', 'MAINLOGGER'
    auto& mainlogger = LogEngine::GetLogger("MainLogger"); 
    mainlogger.Error("This message will be sent into two targets: file(s1) and console (s2 stdout).");
    mainlogger.Info("This message will be sent into file(s1) only because console sink has Loglevel=Error");

    // this logger contains two sinks of File and Stdout types.
    // stdout with name 's2' sink is shared with Mainlogger above
    auto& second = LogEngine::GetLogger("SECOND");
    second.Error("This message will be sent into three targets: file(s3), console (s2 stdout) and rotating file(rotating).");
second.Warn("This message will be sent into two targets file(s3) and rotating file(rotating) because console sink has Loglevel=Error");
}
```

---
#### Asynchronous logging
```c++
    // in Async mode all log messages are added into queue in memory and written to file (or sent to other targets) by a separate thread.
    // it means that main thread does not wait untill log message written to the file, that minimizes effect from log calls 
    auto& async_logger = LogEngine::GetFileLogger("async_file_logger", LOG_FILES_DIR "async_log.txt");
    async_logger.SetAsyncMode(true);

     for (int i = 1; i < 101; ++i) 
    {
        async_logger.InfoFmt("Async message #{}", i);
    }
```
 
---
#### Custom error handler
```c++
void err_handler_example()
{
    // can be set globally or per logger(logger->set_error_handler(..))
    spdlog::set_error_handler([](const std::string &msg) { spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); });
    spdlog::get("console")->info("some invalid message to trigger an error {}{}{}{}", 3);
}

```
---
## Benchmarks

Below are some [benchmarks](bench/bench.cpp) done in Ubuntu 64 bit, Intel i7-4770 CPU @ 3.40GHz

#### Synchronous mode
```
[info] **************************************************************
[info] Single thread, 1,000,000 iterations
[info] **************************************************************
[info] basic_st         Elapsed: 0.17 secs        5,777,626/sec
[info] rotating_st      Elapsed: 0.18 secs        5,475,894/sec
[info] daily_st         Elapsed: 0.20 secs        5,062,659/sec
[info] empty_logger     Elapsed: 0.07 secs       14,127,300/sec
[info] **************************************************************
[info] C-string (400 bytes). Single thread, 1,000,000 iterations
[info] **************************************************************
[info] basic_st         Elapsed: 0.41 secs        2,412,483/sec
[info] rotating_st      Elapsed: 0.72 secs        1,389,196/sec
[info] daily_st         Elapsed: 0.42 secs        2,393,298/sec
[info] null_st          Elapsed: 0.04 secs       27,446,957/sec
[info] **************************************************************
[info] 10 threads, competing over the same logger object, 1,000,000 iterations
[info] **************************************************************
[info] basic_mt         Elapsed: 0.60 secs        1,659,613/sec
[info] rotating_mt      Elapsed: 0.62 secs        1,612,493/sec
[info] daily_mt         Elapsed: 0.61 secs        1,638,305/sec
[info] null_mt          Elapsed: 0.16 secs        6,272,758/sec
```
#### Asynchronous mode
```
[info] -------------------------------------------------
[info] Messages     : 1,000,000
[info] Threads      : 10
[info] Queue        : 8,192 slots
[info] Queue memory : 8,192 x 272 = 2,176 KB 
[info] -------------------------------------------------
[info] 
[info] *********************************
[info] Queue Overflow Policy: block
[info] *********************************
[info] Elapsed: 1.70784 secs     585,535/sec
[info] Elapsed: 1.69805 secs     588,910/sec
[info] Elapsed: 1.7026 secs      587,337/sec
[info] 
[info] *********************************
[info] Queue Overflow Policy: overrun
[info] *********************************
[info] Elapsed: 0.372816 secs    2,682,285/sec
[info] Elapsed: 0.379758 secs    2,633,255/sec
[info] Elapsed: 0.373532 secs    2,677,147/sec

```

## Documentation

Documentation can be found in the [wiki](https://github.com/gabime/spdlog/wiki) pages.

---

Thanks to [JetBrains](https://www.jetbrains.com/?from=spdlog) for donating product licenses to help develop **spdlog** <a href="https://www.jetbrains.com/?from=spdlog"><img src="logos/jetbrains-variant-4.svg" width="94" align="center" /></a>

