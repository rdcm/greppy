#include "parser.cpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char *argv[]) {
    auto result = parse_command_line_args(argc, argv);
    if (!result) {
        cout << "Error: " << get_error_message(result.error()) << endl;
        return -1;
    }

    cout << "done" << endl;

    return 0;
}