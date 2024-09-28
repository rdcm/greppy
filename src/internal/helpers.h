#pragma once

#include "bounded_queue.h"
#include <filesystem>

void find_files(std::filesystem::path dir_path, BoundedQueue<std::filesystem::path> &queue);

bool is_binary_file(const std::filesystem::path &path);

unsigned int get_cores_count();
