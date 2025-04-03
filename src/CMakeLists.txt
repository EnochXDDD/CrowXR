add_library(${CMAKE_PROJECT_NAME} SHARED
    include/ImageProcess.h
    include/ImageProcess.hpp
    ImageProcess.cpp
)

if(ANDROID)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        android
        log
    )
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBRARIES}
)
