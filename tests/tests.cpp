#include "../src/parser.cpp"
#include <gtest/gtest.h>
#include <vector>

using namespace std;

TEST(ParserTests, NoArgs_For_Null) {
    auto result = parse_command_line_args(0, nullptr);

    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), ParseError::NoArgs);
}

TEST(ParserTests, NoArgs_For_Empty_Args) {
    const char *args[0];

    auto result = parse_command_line_args(0, args);

    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), ParseError::NoArgs);
}

TEST(ParserTests, MissingPattern_If_Called_Without_Args) {
    const char *args[] = {"./greppy"};

    auto result = parse_command_line_args(1, args);

    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), ParseError::MissingPattern);
}

TEST(ParserTests, MissingDirectories_If_Passed_Only_Pattern) {
    const char *args[] = {"./greppy", "any_pattern"};

    auto result = parse_command_line_args(2, args);

    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), ParseError::MissingDirectories);
}

TEST(ParserTests, Ok_If_Passed_At_Least_One_Directory) {
    const char *args[] = {"./greppy", "any_pattern", "any_dir"};

    auto result = parse_command_line_args(3, args);

    EXPECT_TRUE(result);
    EXPECT_EQ(result.value().Pattern, "any_pattern");
    EXPECT_EQ(result.value().Directories.size(), 1);
    EXPECT_EQ(result.value().Directories[0], "any_dir");
}

TEST(ParserTests, Ok_If_Passed_Several_Directories) {
    const char *args[] = {"./greppy", "any_pattern", "first_dir", "second_dir"};

    auto result = parse_command_line_args(4, args);

    EXPECT_TRUE(result);
    EXPECT_EQ(result.value().Pattern, "any_pattern");
    EXPECT_EQ(result.value().Directories.size(), 2);
    EXPECT_EQ(result.value().Directories[0], "first_dir");
    EXPECT_EQ(result.value().Directories[1], "second_dir");
}