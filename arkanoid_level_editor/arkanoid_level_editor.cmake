file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(arkanoid_level_editor ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

set_target_properties(arkanoid_level_editor PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(arkanoid_level_editor PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/src
	${CMAKE_SOURCE_DIR}/arkanoid_level_editor/src
	${CMAKE_SOURCE_DIR}/external/imgui # for imgui backend include
	${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(arkanoid_level_editor PRIVATE arkanoid_lib SDL3 SDL3_image imgui)

target_link_directories(arkanoid_level_editor PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")

target_link_directories(arkanoid_level_editor PRIVATE
    "${CMAKE_SOURCE_DIR}/external/SDL3/lib/"
    "${CMAKE_SOURCE_DIR}/external/SDL3_image/lib/"
)

add_custom_command(TARGET arkanoid_level_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL3/lib/SDL3.dll"
        "$<TARGET_FILE_DIR:arkanoid_level_editor>"
)

add_custom_command(TARGET arkanoid_level_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL3_image/lib/SDL3_image.dll"
        "$<TARGET_FILE_DIR:arkanoid_level_editor>"
)

add_custom_command(TARGET arkanoid_level_editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:arkanoid_level_editor>/assets"
)

set_property(TARGET arkanoid_level_editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:arkanoid_level_editor>")