#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

enum class ParseError { NoArgs, MissingPattern, MissingDirectories, NotDirectory };

struct Settings {
  public:
    Settings(std::string pattern, std::filesystem::path directory);

    const std::string Pattern;
    const std::filesystem::path Directory;

    Settings() = delete;
    Settings &operator=(const Settings &) = delete;
};

std::string get_error_message(ParseError error);

std::expected<Settings, ParseError> parse_command_line_args(int argc, const char *argv[]);
