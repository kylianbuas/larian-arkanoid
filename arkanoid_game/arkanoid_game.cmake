file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(arkanoid_game ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "" FILES ${HEADERS} ${SOURCES})

set_target_properties(arkanoid_game PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(arkanoid_game PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/src
	${CMAKE_SOURCE_DIR}/arkanoid_game/src
	${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(arkanoid_game PRIVATE arkanoid_lib SDL3 SDL3_image SDL3_ttf)

target_link_directories(arkanoid_game PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../$<CONFIG>")

target_link_directories(arkanoid_game PRIVATE
    "${CMAKE_SOURCE_DIR}/external/SDL3/lib/"
    "${CMAKE_SOURCE_DIR}/external/SDL3_image/lib/"
    "${CMAKE_SOURCE_DIR}/external/SDL3_ttf/lib/"
)

add_custom_command(TARGET arkanoid_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL3/lib/SDL3.dll"
        "$<TARGET_FILE_DIR:arkanoid_game>"
)

add_custom_command(TARGET arkanoid_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL3_image/lib/SDL3_image.dll"
        "$<TARGET_FILE_DIR:arkanoid_game>"
)

add_custom_command(TARGET arkanoid_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/external/SDL3_ttf/lib/SDL3_ttf.dll"
        "$<TARGET_FILE_DIR:arkanoid_game>"
)

add_custom_command(TARGET arkanoid_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:arkanoid_game>/assets"
)

set_property(TARGET arkanoid_game PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:arkanoid_game>")