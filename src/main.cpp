#include "headers/bounded_queue.h"
#include "headers/parser.h"
#include "headers/utils.h"
#include <filesystem>
#include <iostream>

using namespace std;

const size_t QUEUE_CAPACITY = 1000;

int main_internal(int argc, const char *argv[]) {
    auto result = parse_command_line_args(argc, argv);
    if (!result) {
        cout << "Error: " << get_error_message(result.error()) << '\n';
        return EXIT_FAILURE;
    }

    auto settings = result.value();
    BoundedQueue<filesystem::path> queue(QUEUE_CAPACITY);

    thread find_files_thread(find_files, ref(settings.Directory), ref(queue));

    mutex cout_mutex;
    vector<thread> workers;
    for (size_t i = 0; i < get_cores_count(); i++) {
        thread worker_tread(find_match, ref(settings.Pattern), ref(queue), ref(cout_mutex));
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
        cerr << "Exception caught: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        cerr << "Unknown exception caught" << '\n';
        return EXIT_FAILURE;
    }
}
