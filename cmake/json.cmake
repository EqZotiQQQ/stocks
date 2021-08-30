cmake_minimum_required(VERSION 3.19)

message(STATUS "FetchContent: json")

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.10.0)

FetchContent_MakeAvailable(json)