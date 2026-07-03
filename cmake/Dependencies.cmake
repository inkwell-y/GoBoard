include(FetchContent)

set(GOBOARDCONTROLLER_JUCE_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/JUCE"
    CACHE PATH
    "Path to a local JUCE checkout. If missing, JUCE will be fetched from GitHub.")

if (EXISTS "${GOBOARDCONTROLLER_JUCE_SOURCE_DIR}/CMakeLists.txt")
    message(STATUS "Using local JUCE from ${GOBOARDCONTROLLER_JUCE_SOURCE_DIR}")
    add_subdirectory("${GOBOARDCONTROLLER_JUCE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/JUCE")
else()
    message(STATUS "Local JUCE checkout not found. Fetching JUCE with FetchContent.")

    FetchContent_Declare(
        juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 8.0.8
        GIT_SHALLOW TRUE)

    FetchContent_MakeAvailable(juce)
endif()
