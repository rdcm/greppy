#include "../headers/parser.h"
#include <filesystem>
#include <string>
#include <vector>

Settings::Settings(std::string pattern, std::filesystem::path directory)
    : Pattern(std::move(pattern)), Directory(std::move(directory)) {}

std::string get_error_message(ParseError error) {
    switch (error) {
    case ParseError::NoArgs:
        return "Args not exists";

    case ParseError::MissingPattern:
        return "Search pattern must be specified";

    case ParseError::MissingDirectories:
        return "At least one directory for search must be specified";

    case ParseError::NotDirectory:
        return "Specified path must be a directory";

    default:
        return "Unknown error";
    }
}

std::expected<Settings, ParseError> parse_command_line_args(int argc, const char *argv[]) {
    if (argc == 0) {
        return std::unexpected(ParseError::NoArgs);
    }

    if (argc == 1) {
        return std::unexpected(ParseError::MissingPattern);
    }

    if (argc == 2) {
        return std::unexpected(ParseError::MissingDirectories);
    }

    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.push_back(std::string(argv[i]));
    }

    auto directory_path = std::filesystem::path(args.back());
    if (!std::filesystem::is_directory(directory_path)) {
        return std::unexpected(ParseError::NotDirectory);
    }

    return Settings(args[1], directory_path);
}
