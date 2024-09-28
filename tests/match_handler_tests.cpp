#include "../src/internal/bounded_queue.h"
#include "../src/internal/match_handler.h"
#include <thread>
#include <gtest/gtest.h>

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

    std::string str() const {
        return ss.str();
    }

private:
    std::streambuf *old_stdout;
    std::ostringstream ss;
};

TEST(MatchHandlerTests, Match_Found) {
    std::mutex mutex;
    BoundedQueue<std::filesystem::path> queue(10);

    queue.push("assets/text_file");
    queue.complete();
    {
        StdoutCapture capture;
        find_match("text", queue, mutex);

        EXPECT_TRUE(capture.str().find("Match found in file: \"assets/text_file\" -> text") != std::string::npos);
    }
    

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.is_completed());
}

TEST(MatchHandlerTests, Match_Not_Found) {
    std::mutex mutex;
    BoundedQueue<std::filesystem::path> queue(10);

    queue.push("assets/text_file");
    queue.complete();
    {
        StdoutCapture capture;
        find_match("foo", queue, mutex);

        EXPECT_FALSE(capture.str().find("Match found in file: \"assets/text_file\" -> text") != std::string::npos);
    }
    

    EXPECT_EQ(queue.size(), 0);
    EXPECT_TRUE(queue.is_completed());
}