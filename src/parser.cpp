#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

using namespace std;

enum ParseError { NoArgs, MissingPattern, MissingDirectories, NotDirectory };

struct Settings {
  public:
    Settings(string pattern, filesystem::path directory) : Pattern(pattern), Directory(directory) {}

    const string Pattern;
    const filesystem::path Directory;

    Settings() = delete;
    Settings &operator=(const Settings &) = delete;
};

string get_error_message(ParseError error) {
    switch (error) {
    case ParseError::NoArgs:
        return "Args not exists";

    case ParseError::MissingPattern:
        return "Search pattern must be specified";

    case ParseError::MissingDirectories:
        return "At least one directory for search must be specififed";

    case ParseError::NotDirectory:
        return "Specified path must be a directory";

    default:
        break;
    }
}

expected<Settings, ParseError> parse_command_line_args(int argc, const char *argv[]) {
    if (argc == 0) {
        return unexpected(ParseError::NoArgs);
    }

    if (argc == 1) {
        return unexpected(ParseError::MissingPattern);
    }

    if (argc == 2) {
        return unexpected(ParseError::MissingDirectories);
    }

    vector<string> args;
    for (size_t i = 0; i < argc; i++) {
        args.push_back(string(argv[i]));
    }

    auto directory_path = filesystem::path(args.back());
    if (!filesystem::is_directory(directory_path)) {
        return unexpected(ParseError::NotDirectory);
    }

    args.erase(args.begin());

    return Settings(args.front(), directory_path);
}