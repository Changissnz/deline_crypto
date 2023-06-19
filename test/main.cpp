
#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);


    /// TARGET BELOW 
    ::testing::GTEST_FLAG(filter) = "-Deline22MD_DelineateFull__TerminationCase1_Test*";
    //::testing::GTEST_FLAG(filter) = "DMDTraveller_AlterDeline22__Case2_Test*";

    //::testing::GTEST_FLAG(filter) = "N1n1_RunContractProcedureThenBreak_Case2*";
    return RUN_ALL_TESTS();
}
