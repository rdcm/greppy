#include <expected>
#include <string>
#include <vector>

using namespace std;

enum ParseError { NoArgs, MissingPattern, MissingDirectories };

struct Settings {
  public:
    Settings(string pattern, vector<string> directories)
        : Pattern(pattern), Directories(directories) {}

    const string Pattern;
    const vector<string> Directories;

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

    args.erase(args.begin());
    string pattern = args.front();
    args.erase(args.begin());

    return Settings(pattern, args);
}