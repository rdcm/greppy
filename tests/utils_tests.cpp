#include "../src/headers/bounded_queue.h"
#include "../src/headers/utils.h"
#include <gtest/gtest.h>
#include <thread>

class StdoutCapture {
  public:
    StdoutCapture() {
        // Redirect stdout to a string stream
        old_stdout = std::cout.rdbuf();
        std::cout.rdbuf(ss.rdbuf());
    }

    ~StdoutCapture() {
        // Restore the original stdout
        std::cout.rdbuf(old_stdout);
    }

    std::string str() const { return ss.str(); }

  private:
    std::streambuf *old_stdout;
    std::ostringstream ss;
};

TEST(UtilsTests, Match_Found) {
    std::mutex mutex;
    BoundedQueue<std::filesystem::path> queue(10);

    queue.push("assets/text_file");
    queue.complete();
    {
        StdoutCapture capture;
        find_match("text", queue, mutex);

        EXPECT_TRUE(capture.str().find("Match found in file: \"assets/text_file\" -> text") !=
                    std::string::npos);
    }

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.is_completed());
}

TEST(UtilsTests, Match_Not_Found) {
    std::mutex mutex;
    BoundedQueue<std::filesystem::path> queue(10);

    queue.push("assets/text_file");
    queue.complete();
    {
        StdoutCapture capture;
        find_match("foo", queue, mutex);

        EXPECT_FALSE(capture.str().find("Match found in file: \"assets/text_file\" -> text") !=
                     std::string::npos);
    }

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.is_completed());
}

TEST(UtilsTests, Is_Binary_File) {
    auto result = is_binary_file("assets/bin_file.png");

    EXPECT_TRUE(result);
}

TEST(UtilsTests, Is_Text_File) {
    auto result = is_binary_file("assets/text_file");

    EXPECT_FALSE(result);
}

TEST(UtilsTests, Cores_Count_Ne_Zero) {
    auto result = get_cores_count();

    EXPECT_NE(result, 0);
}

TEST(UtilsTests, Found_Only_Text_File) {
    BoundedQueue<std::filesystem::path> queue(10);
    find_files("assets", queue);

    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.pop().value(), "assets/text_file");
    EXPECT_EQ(queue.size(), 0);
}
