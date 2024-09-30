#include "../src/headers/bounded_queue.h"
#include "../src/headers/helpers.h"
#include <gtest/gtest.h>

TEST(HelpersTests, Is_Binary_File) {
    auto result = is_binary_file("assets/bin_file.png");

    EXPECT_TRUE(result);
}

TEST(HelpersTests, Is_Text_File) {
    auto result = is_binary_file("assets/text_file");

    EXPECT_FALSE(result);
}

TEST(HelpersTests, Cores_Count_Ne_Zero) {
    auto result = get_cores_count();

    EXPECT_NE(result, 0);
}

TEST(HelperTests, Found_Only_Text_File) {
    BoundedQueue<std::filesystem::path> queue(10);
    find_files("assets", queue);

    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.pop().value(), "assets/text_file");
    EXPECT_EQ(queue.size(), 0);
}