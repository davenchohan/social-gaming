#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "CustomExceptions.h"
#include <string>

using namespace testing;

TEST(CustomExceptionsTests, TestUnknownGameExceptionMessage){
    try{
        throw UnknownGameException("Chess");
    } catch (UnknownGameException &e){
        std::string expected = "Error, Unknown Game: Chess";
        std::string generated = e.what();
        EXPECT_EQ(expected, generated);
    }
    catch(...){
        FAIL() << "Expected UnknownGameException";
    }
}

TEST(CustomExceptionsTests, TestIncompleteGameExceptionMessage){
    try{
        throw IncompleteGameException("Chess");
    } catch (IncompleteGameException &e){
        std::string expected = "Error, Incomplete game spec filled for: Chess";
        std::string generated = e.what();
        EXPECT_EQ(expected, generated);
    }
    catch(...){
        FAIL() << "Expected IncompleteGameException";
    }
}

TEST(CustomExceptionsTests, TestUnknownGameSpecExceptionMessage){
    try{
        throw UnknownGameSpecException("badRule");
    }catch (UnknownGameSpecException &e){
        std::string expected = "Error, Unknown game spec: badRule";
        std::string generated = e.what();
        EXPECT_EQ(expected, generated);
    }
    catch(...){
        FAIL() << "Expected UnknownGameSpecException";
    }
}

TEST(CustomExceptionsTests, TestUnknownPlayerExceptionMessage){
    try{
        throw UnknownPlayerException("Gabe");
    }catch (UnknownPlayerException &e){
        auto expected = "Error, player not found: Gabe";
        auto generated = e.what();
        EXPECT_EQ(expected, generated);
    }
    catch (...){
        FAIL() << "Expected UnknownPlayerException";
    }
}

TEST(CustomExceptionsTests, TestUnknownRequestExceptionMessage){
    try{
        throw UnknownRequestException("request AI");
    }catch (UnknownRequestException &e){
        auto expected = "Error, invalid request: request AI";
        auto generated = e.what();
        EXPECT_EQ(expected, generated);
    }
    catch(...){
        FAIL() << "Expected UnknownRequestException";
    }
}