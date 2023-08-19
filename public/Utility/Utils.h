#pragma once
#include <mutex>
#include <string>
#include <vector>

void export_hashes_to_file(const char* file_name, const char* enum_name, std::mutex& mut, std::vector<std::string> input);
void raw_write_to_file(const char* file_name, const char* msg);
