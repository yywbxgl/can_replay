cmake_minimum_required(VERSION 3.14)

project(can_replay)

file(GLOB_RECURSE GENERATED_SOURCE_FILES 
	idl/*.cxx
)


add_executable(can_replay  ${GENERATED_SOURCE_FILES} 
    src/main.cpp
    src/dds_helper.cpp
    src/socket_can.cpp
)

target_include_directories(can_replay PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/idl
)
target_link_libraries(can_replay fastrtps fastcdr)

install(TARGETS can_replay DESTINATION bin)

