cmake_minimum_required(VERSION 3.14)

project(can_replay)

file(GLOB_RECURSE GENERATED_SOURCE_FILES  generated/*.cxx)

add_executable(can_replay  
    ${GENERATED_SOURCE_FILES} 
    src/main.cpp
    src/dds_helper.cpp
)
target_include_directories(can_replay PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
)
target_link_libraries(can_replay fastrtps fastcdr)
install(TARGETS can_replay DESTINATION bin)


# # --------test---------------
add_executable(can_test
    ${GENERATED_SOURCE_FILES} 
    src/can_test.cpp
    src/dds_helper.cpp
)
target_include_directories(can_test PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
)
target_link_libraries(can_test fastrtps fastcdr)
install(TARGETS can_test DESTINATION bin)
