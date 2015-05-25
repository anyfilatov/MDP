#pragma once

#include <iostream>
#include <sstream>
#include <QString>
#include <mutex>
#include <QFile>
#include <memory>

#define INST_LOGGGER()Logger* Logger::logger = nullptr;\
                      std::mutex  Logger::loggerMutex;

class Logger {
    typedef std::mutex Mutex;
    typedef std::unique_lock<Mutex> WriteLock;
public:
    static Logger& get() {
        if( !logger) {
            WriteLock lock(loggerMutex);
            if(!logger){
                logger = new Logger();
            }
        }
        return *logger;
    }
    
    static void addConsoleLogger() {
        get().initConsoleLogger();
    }
    
    static void addFileLogger(const QString& fileName) {
        get().addLogger(fileName);
    }
    
    void initConsoleLogger() {
        consoleLogger_ = std::make_shared<LogConsole>();
    }

    void addLogger(const QString& fileName) {
        auto fileLogger = std::make_shared<LogFile>(fileName);
        fileLoggers_.push_back(fileLogger);
    }

    void write(const std::string& message) {
        WriteLock lock(mutex_);
        if (consoleLogger_) {
            consoleLogger_->write(message);
        }
        for (auto& l : fileLoggers_) {
            l->write(message);
        }
    }
private:
    static Logger* logger;
    static std::mutex loggerMutex;
    Logger(){}
private:
    class LogFile{
    public:
        LogFile(QString fileName) : fileName_(fileName), file_(fileName_) {}
        LogFile(){}
        void write(const std::string& message) {
            if(!isOpen) {
                file_.open(QIODevice::WriteOnly);
                isOpen = true;
            }
            file_.write(message.c_str(), message.size());
            file_.write("\n", 1);
            file_.flush();
        }
        ~LogFile(){
            file_.close();
        }
    private:
        QString fileName_;
        QFile file_;
        bool isOpen = false;
    };
    
    class LogConsole{
    public:
        LogConsole(){}
        void write(const std::string& message) {
            std::cout << message << std::endl << std::flush;
        }
    };
private:
    std::vector<std::shared_ptr<LogFile>> fileLoggers_;
    std::shared_ptr<LogConsole> consoleLogger_;
    Mutex mutex_;
    
};

#define LOG_LEVEL_FATAL    " FATAL "//(0)
#define LOG_LEVEL_ERROR    " ERROR "//(1)
#define LOG_LEVEL_WARNING  "WARNING"//(2)
#define LOG_LEVEL_INFO     " INFO  "//(3)
#define LOG_LEVEL_DEBUG    " DEBUG "//(4)
#define LOG_LEVEL_TRACE    " TRACE "//(5)

#define LOG(LEVEL, CHAIN) {\
                std::stringstream ss;\
                ss << "[" << LEVEL << "] " << CHAIN << "        |" << __FILE__ << " +" << __LINE__;\
                Logger::get().write(ss.str());\
            }

#define LOG_FATAL(CHAIN)   LOG(LOG_LEVEL_FATAL, CHAIN)
#define LOG_ERROR(CHAIN)   LOG(LOG_LEVEL_ERROR, CHAIN)
#define LOG_WARNING(CHAIN) LOG(LOG_LEVEL_WARNING, CHAIN)
#define LOG_INFO(CHAIN)    LOG(LOG_LEVEL_INFO, CHAIN)
//#ifdef DEBUG
#define LOG_DEBUG(CHAIN)   LOG(LOG_LEVEL_DEBUG, CHAIN)
//#else
//#define LOG_DEBUG(CHAIN)
//#endif
#define LOG_TRACE(CHAIN)   LOG(LOG_LEVEL_TRACE, CHAIN)
//#define LOG_TRACE(CHAIN)

