#pragma once
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

#include "../native/natives.h"

using namespace std;

enum class LogLevel: int
{
    DEBUG = 1,
    INFO = 0,
    WARNING = 2,
    FATAL = 3
};

struct Vector3;

class RdrLogger
{
private:
    LogLevel level_;
    std::ofstream file;
    std::mutex log_mutex_;
    const char* file_name;
    std::string getLogLevelString(LogLevel level);
    
protected:
   
    void log(const char* msg, LogLevel level);
    void log(Vector3 position, LogLevel level);
    void log(string msg, LogLevel level);
    RdrLogger(const char* fileName, LogLevel level);

public:
    static std::unordered_map<std::string, RdrLogger*> instances;
    
    RdrLogger(RdrLogger &other) = delete;             // Disable clone
    void operator=(const RdrLogger &) = delete;    // Disable assignment


    static RdrLogger *get_logger(const char* fileName, LogLevel level);

    void raw_write_to_log(const char* msg);
    
    void write_to_log(const char* msg);

    void set_level(LogLevel level);

    void set_file_name(const char* fileName);

    void info(const char* msg);

    void info(stringstream str);

    void debug(const char* str);

    void warning(const char* msg);

    void fatal(const char* msg);

    void log_vector(Vector3 vector);

    
};

void displayDebugText(const char* text, float offset = 0.0f);
