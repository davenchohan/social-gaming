#include "ExecutionQueue.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <string>
#include <map>
#include <vector>

using namespace testing;

/*
These unit tests assume the integrity of unique ptr in queue is upheld.
*/

class FakeExecutionNode : public ExecutionNode{
public:
    FakeExecutionNode(std::string id) : identifier(std::move(id)) {}
    void print() override {
        std::cout << "Fake Execution Node" << std::endl;
    }
private:
    std::string identifier;
};

class FakeExecutionNode2 : public ExecutionNode{
public: 
    FakeExecutionNode2(std::string id) : identifier(std::move(id)) {}
    void print() override { 
        std::cout << "Fake Execution Node2" << std::endl;
    }
private:
    std::string identifier;
};

TEST(ExecutionNodeQueueTests, TestEmptyQueue){
    ExecutionQueue Q;
    EXPECT_TRUE(Q.empty());
    ASSERT_EQ(nullptr, Q.pop());
}

TEST(ExecutionNodeQueueTests, TestBasicQueue){
    ExecutionQueue Q;
    Q.push(std::make_unique<FakeExecutionNode>("1234"));
    ASSERT_EQ(Q.getSize(), 1);
}

TEST(ExecutionNodeQueueTests, TestBasicPushPop){
    ExecutionQueue Q;
    Q.push(std::make_unique<FakeExecutionNode>("1111"));
    Q.push(std::make_unique<FakeExecutionNode>("1234"));
    Q.push(std::make_unique<FakeExecutionNode>("5555"));
    auto item = Q.pop();
    EXPECT_TRUE(item != nullptr);
    ASSERT_EQ(Q.getSize(), 2);
}

TEST(ExecutionNodeQueueTests, TestPushPop2){
    ExecutionQueue Q;
    Q.push(std::make_unique<FakeExecutionNode>("1111"));
    auto item = Q.pop();
    Q.push(std::make_unique<FakeExecutionNode>("1234"));
    auto item2 = Q.pop();
    auto item3 = Q.pop();
    Q.push(std::make_unique<FakeExecutionNode>("5555"));
    EXPECT_TRUE(item3 == nullptr);
    EXPECT_TRUE(item2 != item3);
    ASSERT_EQ(Q.getSize(), 1);
}

TEST(ExecutionNodeQueueTests, TestDifferentTypes){
    ExecutionQueue Q;
    EXPECT_NO_THROW({
        Q.push(std::make_unique<FakeExecutionNode>("1111"));
        Q.push(std::make_unique<FakeExecutionNode2>("1123"));
    });
    ASSERT_EQ(Q.getSize(), 2);
}
