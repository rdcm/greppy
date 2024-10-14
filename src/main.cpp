#include "headers/bounded_queue.h"
#include "headers/parser.h"
#include "headers/utils.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>

constexpr size_t QUEUE_CAPACITY = 1000;

int main_internal(int argc, const char *argv[]) {
    auto result = parse_command_line_args(argc, argv);
    if (!result) {
        std::cout << "Error: " << get_error_message(result.error()) << '\n';
        return EXIT_FAILURE;
    }

    auto settings = result.value();
    BoundedQueue<std::filesystem::path> queue(QUEUE_CAPACITY);

    std::thread find_files_thread(find_files, std::ref(settings.Directory), std::ref(queue));

    std::mutex cout_mutex;
    std::vector<std::thread> workers;
    for (size_t i = 0; i < get_cores_count(); i++) {
        std::thread worker_tread(find_match, ref(settings.Pattern), std::ref(queue), std::ref(cout_mutex));
        workers.push_back(std::move(worker_tread));
    }

    find_files_thread.join();
    for (auto &worker_thread : workers) {
        worker_thread.join();
    }

    return EXIT_SUCCESS;
}

int main(int argc, const char *argv[]) {
    try {
        return main_internal(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Exception caught: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception caught" << '\n';
        return EXIT_FAILURE;
    }
}
