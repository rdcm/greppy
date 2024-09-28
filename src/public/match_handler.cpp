#include "../internal/match_handler.h"
#include "../internal/bounded_queue.h"
#include <optional>
#include <iostream>
#include <regex>
#include <fstream>
#include <thread>

void find_match(std::string pattern, BoundedQueue<std::filesystem::path> &queue, std::mutex &cout_mutex) {
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
                std::cout << "Match found in file: " << get<0>(m) << " -> " << get<1>(m) << std::endl;
            }
        }

        if (queue.is_completed() && !file_path_opt) {
            break;
        }
    }
}