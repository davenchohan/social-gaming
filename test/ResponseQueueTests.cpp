#include "ResponseQueue.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <map>
#include <vector>

using namespace testing;


TEST(ResponseQueueTests, TestBasicPush){
    ResponseQueue someQ;
    std::string someItem = "I am some item";
    someQ.push(someItem);
    auto generated = someQ.Front();
    ASSERT_EQ(generated, someItem);
}

TEST(ResponseQueueTests, TestBasicPop){
    ResponseQueue someQ;
    std::string item1 = "Hello";
    someQ.push(item1);
    auto generated = someQ.pop();
    ASSERT_EQ(item1, generated);
}

TEST(ResponseQueueTests, TestGetAll){
    ResponseQueue someQ;
    std::vector<std::string> expected = {"item1", "item2", "item3"};
    someQ.push("item1");
    someQ.push("item2");
    someQ.push("item3");
    auto generated = someQ.getAllMessages();
    ASSERT_EQ(generated, expected);
}

TEST(ResponseQueueTests, TestMultiplePushPop){
    ResponseQueue someQ;
    std::vector<std::string> expected = {"Hello", "World"};
    someQ.push("Dummy");
    someQ.push("Hello");
    someQ.push("World");
    someQ.pop();
    auto generated = someQ.getAllMessages();
    ASSERT_EQ(generated, expected);
}

TEST(ResponseQueueTests, TestEmptyPop){
    ResponseQueue someQ;
    auto generated = someQ.pop();
    ASSERT_EQ(generated, "");
}

TEST(ResponseQueueTests, TestEmptyFront){
    ResponseQueue someQ;
    auto generated = someQ.Front();
    ASSERT_EQ(generated, "");
}