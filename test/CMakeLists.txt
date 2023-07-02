enable_testing()
find_package(GTest REQUIRED)

# Add source directories
file(GLOB TESTFILES "*.cpp")
add_executable(deline_crypto_test ${TESTFILES})

find_package(Armadillo REQUIRED)

target_include_directories(deline_crypto_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(deline_crypto_test deline_crypto_lib ${GTEST_LIBRARIES} ${ARMADILLO_LIBRARIES} pthread) 
