#pragma once

#include "bounded_queue.h"
#include <filesystem>
#include <string>
#include <thread>

void find_match(std::string pattern, BoundedQueue<std::filesystem::path> &queue,
                std::mutex &cout_mutex);