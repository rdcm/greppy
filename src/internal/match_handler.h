#pragma once

#include "bounded_queue.h"
#include <string>
#include <thread>
#include <filesystem>

void find_match(std::string pattern, BoundedQueue<std::filesystem::path> &queue, std::mutex &cout_mutex);