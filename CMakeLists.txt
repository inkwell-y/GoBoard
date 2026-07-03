cmake_minimum_required(VERSION 3.25)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GoBoardControllerVersion)
include(JUCEDefaults)

set(PROJECT_NAME "GoBoardController")
set(PRODUCT_NAME "Go Board Controller")
set(COMPANY_NAME "GoBoard")
set(BUNDLE_ID "com.goboard.goboardcontroller")
set(FORMATS Standalone VST3)

project(${PROJECT_NAME} VERSION ${GO_BOARD_CONTROLLER_VERSION})

include(Dependencies)
include(CTest)

juce_add_plugin("${PROJECT_NAME}"
    COMPANY_NAME "${COMPANY_NAME}"
    BUNDLE_ID "${BUNDLE_ID}"
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE GbRd
    PLUGIN_CODE GbCt
    FORMATS ${FORMATS}
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    PRODUCT_NAME "${PRODUCT_NAME}")

add_library(SharedCode INTERFACE)

set(SourceFiles
    source/PluginEditor.cpp
    source/PluginEditor.h
    source/PluginProcessor.cpp
    source/PluginProcessor.h
    source/core/BoardState.cpp
    source/core/BoardState.h
    source/core/ControlVector.h
    source/core/FeatureExtractor.cpp
    source/core/FeatureExtractor.h
    source/core/MappingEngine.cpp
    source/core/MappingEngine.h
    source/gui/BoardComponent.cpp
    source/gui/BoardComponent.h)

set(CoreSourceFiles
    source/core/BoardState.cpp
    source/core/BoardState.h
    source/core/ControlVector.h
    source/core/FeatureExtractor.cpp
    source/core/FeatureExtractor.h
    source/core/MappingEngine.cpp
    source/core/MappingEngine.h)

target_sources(SharedCode INTERFACE ${SourceFiles})

target_include_directories(SharedCode
    INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/source")

target_compile_definitions(SharedCode
    INTERFACE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    GO_BOARD_CONTROLLER_PLACEHOLDER_BUILD=1
    GO_BOARD_CONTROLLER_VERSION_STRING="${PROJECT_VERSION}")

target_link_libraries(SharedCode
    INTERFACE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_gui_extra
    juce::juce_osc
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    SharedCode
    juce::juce_osc)

if (BUILD_TESTING)
    add_executable(GoBoardControllerTests
        tests/TestMain.cpp
        tests/BoardStateTests.cpp
        tests/FeatureExtractorTests.cpp
        tests/MappingEngineTests.cpp
        ${CoreSourceFiles})

    target_include_directories(GoBoardControllerTests
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/source")

    target_compile_definitions(GoBoardControllerTests
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

    target_link_libraries(GoBoardControllerTests
        PRIVATE
        juce::juce_core
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

    add_test(NAME GoBoardControllerTests COMMAND GoBoardControllerTests)
endif()
