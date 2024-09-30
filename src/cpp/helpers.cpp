#include "../headers/helpers.h"
#include "../headers/bounded_queue.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

void find_files(std::filesystem::path dir_path, BoundedQueue<std::filesystem::path> &queue) {
    auto now = std::chrono::system_clock::now();
    for (const auto &entry : std::filesystem::recursive_directory_iterator(dir_path)) {
        auto path = entry.path();
        if (entry.is_regular_file() && !is_binary_file(path)) {
            auto last_write_time = std::filesystem::last_write_time(path);
            if (last_write_time.time_since_epoch() > now.time_since_epoch()) {
                continue;
            }

            queue.push(path);
        }
    }

    queue.complete();
}

bool is_binary_file(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    if (file) {
        char ch;
        // try find non-text character
        while (file.read(&ch, 1)) {
            if (iscntrl(static_cast<unsigned char>(ch)) &&
                !isspace(static_cast<unsigned char>(ch))) {
                return true;
            }
        }
    }
    return false;
}

unsigned int get_cores_count() {
    unsigned int cores_count = std::thread::hardware_concurrency();
    if (cores_count == 0) {
        // https://en.cppreference.com/w/cpp/thread/thread/hardware_concurrency
        return 1;
    }

    return cores_count;
}