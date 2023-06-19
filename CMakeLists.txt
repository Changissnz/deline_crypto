cmake_minimum_required(VERSION 3.0)

project(deline_crypto)

# include(build/cmake/GolangSetup.cmake) 
add_subdirectory(src) 
add_subdirectory(include) 
add_subdirectory(test)

# add deline_crypto library 
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h" "include/*.hpp")
add_library(deline_crypto_lib ${SOURCES})

#message("FINDING LIBRARIO: ${CMAKE_CURRENT_SOURCE_DIR}/include")
#set_target_properties(golemLib PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/include/libgolem_exec.so")

# include header folder 
target_include_directories(deline_crypto_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR}/include) 
add_executable(deline_crypto_exec "src/main.cpp")

# link the NVMBLib library and golem library to the NVMB_EXEC executable 
target_link_libraries(deline_crypto_exec PUBLIC deline_crypto_lib)

# use below for dynload 
#target_link_libraries(NVMB_EXEC PUBLIC ${CMAKE_DL_LIBS})
