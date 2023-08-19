#pragma once
#include <deque>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>

class RdrConsole
{
private:
    std::deque<std::string> console_lines_;
    std::string static_line_;
    std::chrono::steady_clock::time_point static_text_creation_ts_;
    std::mutex update_mutex;
public:

    static RdrConsole& get_instance()
    {
        static RdrConsole instance;
        return instance;
    }

    void write_to_console(std::string text);
    void write_static_line(std::string static_line);
    void update();
};
