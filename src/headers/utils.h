#pragma once

#include "bounded_queue.h"
#include <filesystem>
#include <string>
#include <thread>

void find_files(const std::filesystem::path &dir_path, BoundedQueue<std::filesystem::path> &queue);

bool is_binary_file(const std::filesystem::path &path);

unsigned int get_cores_count();

void find_match(const std::string &pattern, BoundedQueue<std::filesystem::path> &queue,
                std::mutex &cout_mutex);