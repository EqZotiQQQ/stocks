message(CPPCheck)

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
    find_program(CMAKE_CXX_CPPCHECK cppcheck NAMES cppcheck HINTS $ENV{PROGRAMFILES}/cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        add_custom_target(cppcheck
                COMMAND ${CMAKE_CXX_CPPCHECK}
                --enable=warning,performance,portability,information,missingInclude
                --std=c++20
                --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
                --verbose
                --quiet
                SOURCES ${ALL_SOURCE_FILES}
                )
    else()
        message("No Cppcheck detected")
    endif()
elseif(APPLE)
    message("Not done yet")
else()
    find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        add_custom_target(
                cppcheck
                COMMAND /usr/bin/cppcheck
                --enable=warning,performance,portability,information,missingInclude
                --std=c++20
                --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
                --verbose
                --quiet
                ${ALL_SOURCE_FILES}
        )
    endif()

endif()