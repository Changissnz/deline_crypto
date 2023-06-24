
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    /// TARGET BELOW 
    ::testing::GTEST_FLAG(filter) = "-Deline22MD_DelineateFull__TerminationCase1_Test*";
    return RUN_ALL_TESTS();
}
