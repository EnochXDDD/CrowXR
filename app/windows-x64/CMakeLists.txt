add_executable(${CMAKE_PROJECT_NAME}_windows_x64
    main.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}_windows_x64 PRIVATE
    include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME}_windows_x64 PRIVATE
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBRARIES}
)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_windows_x64 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_windows_x64>
    COMMAND ${CMAKE_COMMAND} -E copy ${VCPKG_INSTALLED_DLLS} $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_windows_x64>
    COMMAND_EXPAND_LISTS
)
