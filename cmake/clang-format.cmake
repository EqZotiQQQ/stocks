message(clang-format)

set(EXCLUDE_DIRS cmake-build-debug)
set(UNCHECKED "")
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    string(FIND ${SOURCE_FILE} ${EXCLUDE_DIRS} EXCLUDE_DIRS_FOUND)
    if (NOT ${EXCLUDE_DIRS_FOUND} EQUAL -1)
        set(UNCHECKED "${UNCHECKED} ${SOURCE_FILE}")
        list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
    endif ()
endforeach ()

if (WIN32)
    find_program(CMAKE_CXX_CLANGFORMAT clang-format NAMES clang-format HINTS $ENV{PROGRAMFILES}/clang-format)
    if (CMAKE_CXX_CLANGFORMAT)
        add_custom_target(clang-format
                COMMAND ${CMAKE_CXX_CLANGFORMAT}
                -style=LLVM
                -i
                ${ALL_SOURCE_FILES}
                )
    else ()
        message("No clang-format detected")
    endif ()
elseif(APPLE)
    message("Not done yet")
else()
    find_program(CMAKE_CXX_CLANGFORMAT NAMES clang-format-10)
    if (CMAKE_CXX_CLANGFORMAT)
        add_custom_target(
                clang-format-10
                COMMAND /usr/bin/clang-format-10
                -style=file
                -i
                ${ALL_SOURCE_FILES}
        )
    endif()
endif()

