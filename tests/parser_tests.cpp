#include "../src/internal/parser.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <vector>

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

TEST(ParserTests, NotDirectory_If_Passed_Any_String) {
    const char *args[] = {"./greppy", "any_pattern", "any_dir"};

    auto result = parse_command_line_args(3, args);

    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), ParseError::NotDirectory);
}

TEST(ParserTests, Ok_If_Passed_Correct_Dir_Path) {
    const char *args[] = {"./greppy", "any_pattern", "/"};

    auto result = parse_command_line_args(3, args);

    EXPECT_TRUE(result);
    EXPECT_EQ(result.value().Pattern, "any_pattern");
    EXPECT_EQ(result.value().Directory, std::filesystem::path("/"));
}
