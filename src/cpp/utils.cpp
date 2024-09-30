#include "../headers/utils.h"
#include "../headers/bounded_queue.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <optional>
#include <regex>

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

void find_match(std::string pattern, BoundedQueue<std::filesystem::path> &queue,
                std::mutex &cout_mutex) {
    while (true) {
        std::optional<std::filesystem::path> file_path_opt = queue.pop();
        if (file_path_opt) {
            auto file_path = file_path_opt.value();
            std::ifstream file(file_path, std::ios::in);
            if (!file.is_open()) {
                std::cerr << "Error opening file: " << file_path << std::endl;
                continue;
            }

            std::string line;
            std::regex regex_pattern(pattern);
            std::vector<std::tuple<std::filesystem::path, std::string>> matches;
            while (getline(file, line)) {
                if (regex_search(line, regex_pattern)) {
                    matches.push_back(make_tuple(file_path, line));
                }
            }

            std::lock_guard<std::mutex> guard(cout_mutex);
            for (auto &m : matches) {
                std::cout << "Match found in file: " << get<0>(m) << " -> " << get<1>(m)
                          << std::endl;
            }
        }

        if (queue.is_completed() && !file_path_opt) {
            break;
        }
    }
}