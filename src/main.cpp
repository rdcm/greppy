#include "bounded_queue.cpp"
#include "parser.cpp"
#include <chrono>
#include <coroutine>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono;

const size_t QUEUE_CAPACITY = 1000;

bool is_binary_file(const filesystem::path &path) {
    ifstream file(path, ios::binary);
    if (file) {
        char ch;
        while (file.read(&ch, 1)) {
            if (iscntrl(static_cast<unsigned char>(ch)) &&
                !isspace(static_cast<unsigned char>(ch))) {
                return true; // Found non-text character
            }
        }
    }
    return false;
}

void find_files(filesystem::path dir_path, BoundedQueue<filesystem::path> &queue) {
    auto now = system_clock::now();
    for (const auto &entry : filesystem::recursive_directory_iterator(dir_path)) {
        auto path = entry.path();
        if (entry.is_regular_file() && !is_binary_file(path)) {
            auto last_write_time = filesystem::last_write_time(path);
            if (last_write_time.time_since_epoch() > now.time_since_epoch()) {
                continue;
            }

            queue.push(path);
        }
    }

    queue.complete();
}

void find_match(string pattern, BoundedQueue<filesystem::path> &queue) {
    while (true) {
        if (queue.is_completed()) {
            break;
        }

        optional<filesystem::path> file_path_opt = queue.pop();
        if (file_path_opt) {
            auto file_path = file_path_opt.value();
            ifstream file(file_path, ios::in);
            if (!file.is_open()) {
                cerr << "Error opening file: " << file_path << endl;
            }

            string line;
            regex regex_pattern(pattern);
            while (getline(file, line)) {
                if (regex_search(line, regex_pattern)) {
                    cout << "Match found in file: " << file_path << " -> " << line << endl;
                }
            }
        }
    }
}

int main(int argc, const char *argv[]) {
    auto result = parse_command_line_args(argc, argv);
    if (!result) {
        cout << "Error: " << get_error_message(result.error()) << endl;
        return -1;
    }

    auto settings = result.value();
    BoundedQueue<filesystem::path> queue(QUEUE_CAPACITY);

    thread find_files_tread(find_files, settings.Directory, ref(queue));

    vector<thread> workers;
    for (size_t i = 0; i < thread::hardware_concurrency(); i++) {
        thread worker_tread(find_match, settings.Pattern, ref(queue));
        workers.push_back(std::move(worker_tread));
    }

    find_files_tread.join();
    for (auto &worker_thread : workers) {
        worker_thread.join();
    }

    return 0;
}