option(USE_TEST "Test with test files" OFF)
message(STATUS "source dir:${CMAKE_SOURCE_DIR}")

if(USE_TEST)
    SET(CMAKE_BUILD_TYPE "Debug")
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g2 -g")
    SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
    link_directories("${CMAKE_SOURCE_DIR}/libs")
    add_executable(test_thread test_thread.cc)
    add_executable(test_sem test_sem.cc)
    add_executable(test_cond_var test_cond_var.cc)
    target_include_directories(test_thread PRIVATE ${CMAKE_SOURCE_DIR})
    target_include_directories(test_sem PRIVATE ${CMAKE_SOURCE_DIR})
    target_include_directories(test_cond_var PRIVATE ${CMAKE_SOURCE_DIR})
    target_link_libraries(test_thread yf_thread_shared)
    target_link_libraries(test_sem yf_thread_shared)
    target_link_libraries(test_cond_var yf_thread_shared)
endif(USE_TEST)
