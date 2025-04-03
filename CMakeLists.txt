cmake_minimum_required(VERSION 3.22.1)
project("CrowXR")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

if (WIN32)
    add_compile_definitions(
        UNICODE
        _UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
    set(VCPKG_INSTALLED_DLLS
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\abseil_dll.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\fmtd.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\jpeg62.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\liblzma.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libpng16d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libprotobuf-lited.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libprotobufd.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libprotocd.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libsharpyuv.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libwebp.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libwebpdecoder.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libwebpdemux.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\libwebpmux.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_calib3d4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_core4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_dnn4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_features2d4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_flann4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_highgui4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_imgcodecs4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_imgproc4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_ml4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_objdetect4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_photo4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_stitching4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_video4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\opencv_videoio4d.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\pkgconf-5.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\spdlogd.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\tiffd.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\turbojpeg.dll
        ${VCPKG_INSTALLED_DIR}\\x64-windows\\debug\\bin\\zlibd1.dll
    )
endif()

add_subdirectory(src)
add_subdirectory(app)
