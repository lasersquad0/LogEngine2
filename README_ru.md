# LogEngine2

Автор: Andrey Romanchenko (lasersquad@gmail.com)

Web site: https://github.com/lasersquad0/LogEngine2

Вопросы и отзывы: https://github.com/lasersquad0/LogEngine2/discussions

## Обзор

LogEngine2 — логируй просто и в тоже время гибко.

Основные преимущества:
 * простота
 * лёгкость использования
 * независимость от платформы
 * скорость работы
 * небольшой объём исходного кода.

В этих файлах ```testXXX.cpp``` из каталога [test/](test/) найдете примеры использования библиотеки LogEngine2 в дополнение к [примерам](#примеры-использования) ниже.
В файле [example.lfg](test/testFiles/example.lfg) найдете полное описание параметров конфигурационного файла LogEngine2.

Вопросы и сообщения об ошибках приветствуются. 
Используйте систему баг репортинга Github по адресу https://github.com/lasersquad0/LogEngine2/issues.

## Установка
#### Microsoft Windows
Откройте этот проект MSVC [LogEngine.vcproj](prj/msvc/LogEngine/LogEngine.vcproj) в Visual Studio.
Он содержит несколько предопределённых конфигураций, таких как Debug, Release и другие. Вы можете собрать любую из них.
Каждая конфигурация создаёт .lib-файл с уникальными именами (например, ```LogEngine2_64.lib``` для Release или ```LogEngine2d_64.lib``` для Debug).

Вы также можете посмотреть папки [prj/msvc/testLogEngine](prj/msvc/testLogEngine) (или [prj/Builder/testLogEngine2](prj/Builder/testLogEngine2)) с проектами, содержащими примеры использования библиотеки LogEngine2.

#### Другие операционные системы
Для других систем смотрите текстовый файл [INSTALL](INSTALL) с подробностями.

## Платформы
* Windows (Visual Studio 2022)
* Linux и аналогичные

## Возможности
* Разнообразное форматирование с использованием std::format из C++20
* Загрузка конфигурации логгеров из .lfg-файла
* Асинхронный режим (может быть включён и выключен)
* Пользовательский логгер с обратными вызовами
* Обьекты логирования:
  * Обычные лог-файлы
  * Ротируемые лог-файлы
  * Логирование в консоль (stdio и stderr)
  * Символьная строка
* Настраиваемые шаблоны для строк, которые могут включать информацию:
  * Дата и/или время
  * Идентификатор потока (thread id)
  * Маркер уровня логирования или полное/сокращённое название уровня
  * Имя приложения и версия приложения
  * Название ОС и версия ОС
* Фильтрация логов — уровни логирования могут быть изменены как во время выполнения, так и во время компиляции
* Статистика логирования
* Включает удобные классы динамических массивов, которые могут использоваться отдельно от библиотеки LogEngine2

## Примеры использования

#### Простые наиболее частые случаи использования
```c++
#include "LogEngine.h"

int main() 
{
    LogEngine::Logger& logger = LogEngine::GetStdoutLogger("consolelogger");
   
    logger.InfoFmt("Welcome to LogEngine version {}.{}.{}  !",
         LOGENGINE_VER_MAJOR, LOGENGINE_VER_MINOR, LOGENGINE_VER_PATCH);
   
    logger.InfoFmt("Padding in numbers:  {:08d}", 12);
    logger.CritFmt("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger.InfoFmt("Support for floats {:03.2f}", 1.23456);
    logger.InfoFmt("Positional args are {1} {0}..", "too", "supported");
    logger.InfoFmt("{:>8} aligned, {:<8} aligned", "right", "left");

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
}

```

#### Создание логгеров типов: файловый логгер, stdout/stderr
```c++
void stdout_file_example()
{
    auto& stdout_logger = LogEngine::GetStdoutLogger("stdoutlogger");
    stdout_logger.Info("Console stdout logger has created.");

    auto& stderr_logger = LogEngine::GetStderrLogger("stderrlogger");
    stderr_logger.Info("Console stderr logger has created.");

    // Create file logger (not rotated). 
    auto& file_logger = LogEngine::GetFileLogger("file_logger",  "logs/file_log.log");
    file_logger.Info("My file_logger has created.");
 
    //later you can get existing file logger using its name
    auto& file_logger2 = LogEngine::GetLogger("file_logger");
    //file_logger2 is the reference to the same file_logger created above
    file_logger2.Info("My file_logger2 has got."); 
}
```

#### Ротационный логгер, ротация лог файлов
```c++
void rotating_file_example()
{
    // Create a file rotating logger with 1kb size max and time stamps in file names.
    auto& rotLogger = LogEngine::GetRotatingFileLogger("logger_name", "logs/rot.txt", 
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
    auto& rotLogger2 =  LogEngine::GetRotatingFileLogger("logger_name2", "logs/rot2.txt", 
        1024, LogEngine::rsNumbers, 10);
    rotLogger2.Info("Rotating file message");
}
```

#### Два логгера с несколькими общими sinks, каждый sink имеет свой формат и log level.

```c++
void multi_sink_example() 
{
    using namespace LogEngine;
    
    // we need shared_ptr for Sinks here for proper freeing Sink objects 
    // when they shared between several loggers. 
    std::shared_ptr<Sink> consoleSink(new StdoutSink("console_sink"));
    consoleSink->SetLogLevel(Levels::llWarning);
    consoleSink->SetPattern("[multi_sink_example] [%loglevel% %TIME% #%thread%] %Msg%");

    std::shared_ptr<Sink> fileSink(new FileSink("file_sink", "multisink.txt"));
    fileSink->SetLogLevel(Levels::llTrace);

    Logger logger("multi_sink", { consoleSink, fileSink });
    // or we can create global logger 
    // Logger& logger = GetMultiLogger("multi_sink_logger", { consoleSink, fileSink });
		
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
```

#### Загрузка логгеров из файла конфигурации (*.lfg)
```c++
// loads and constructs loggers and sinks from .lfg file.
void lfg_example()
{
    // once loggers are initialized you can use GetLogger() to get needed logger by its name
    LogEngine::InitFromFile("LogExample.lfg");

    
    // this logger contains two sinks of File and Stdout types.
    // when we log into this logger messages will be sent into two targets.
    // logger name is case INSensitive here, so names below will considered as one logger: 
    // 'MainLogger', 'mainlogger', 'MAINLOGGER'
    auto& mainlogger = LogEngine::GetLogger("MainLogger");
    mainlogger.Error("This message will be sent into two targets: file and console (stdout).");
    mainlogger.Info("This message will be sent to file only since console sink has Loglevel=Error");

    // this logger contains two sinks of File and Stdout types.
    // stdout with name 's2' sink is shared with Mainlogger above
    auto& second = LogEngine::GetLogger("SECOND");
    second.Error("This message will be sent into three targets: file, console and rotating file.");
    second.Warn("This message will be sent into two targets file and rotating file");
}
```

#### Асинхронное логирование
```c++
void async_example()
{
    // in Async mode all log messages are added into queue in memory and written to 
    // file (or sent to other targets) by a separate thread. 
    // it means that main thread does not wait until log messages written to the file
    // that minimizes effect from log calls 
    auto& async_logger = LogEngine::GetFileLogger("async_logger", "logs/async_log.txt");
    async_logger.SetAsyncMode(true);

    for (int i = 1; i < 101; ++i) 
    {
        async_logger.InfoFmt("Async message #{}", i);
    }
}    
```
 
#### Пользовательский callback логгер
```c++
void callback_example() 
{
    auto& logger = LogEngine::GetCallbackLogger("custom_callback_logger",
        [](const LogEngine::LogEvent& /*msg*/) 
        {
            std::cout << "Lambdadada " << std::endl;
            // do what you need to do with msg
        });

    logger.Info("Origial message for callback logger");
}

```

## Замеры производительности

Below are some [benchmarks](bench/bench.cpp) done in Ubuntu 64 bit, Intel i7-4770 CPU @ 3.40GHz

#### Synchronous mode
```
[info] **************************************************************
[info] Single thread, 1,000,000 iterations
[info] **************************************************************
[info] **************************************************************
[info] C-string (400 bytes). Single thread, 1,000,000 iterations
[info] **************************************************************
[info] **************************************************************
[info] 10 threads, competing over the same logger object, 1,000,000 iterations
[info] **************************************************************
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
[info] 
[info] *********************************
[info] Queue Overflow Policy: overrun
[info] *********************************

```
	
