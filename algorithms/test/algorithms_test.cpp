#include <gtest/gtest.h>
#include "../include/types.hpp"
#include "../include/algo.hpp"

// Demonstrate some basic assertions.
TEST(THOUSAND_DIGIT_TEST, BasicAssertions) {
    std::string _input = input;
    _input.erase(std::remove(_input.begin(), _input.end(), '\n'), _input.end());

    Array* nums = DigitStringToIntArray(_input);

    int result = GetHighestProduct(nums, 4);
    EXPECT_EQ(result, 5832); // 9 * 9 * 8 * 9 = 5832

    free(nums->data);
    free(nums);
}

