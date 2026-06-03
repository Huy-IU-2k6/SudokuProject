cmake_minimum_required(VERSION 3.15)
project(SudokuPlatform VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tự động quét toàn bộ file .cpp trong src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(SudokuProject ${SOURCES})

# Thư mục include nội bộ của dự án
target_include_directories(SudokuProject PRIVATE src)

# ==========================================
# CẤU HÌNH LIÊN KẾT SFML (CROSS-PLATFORM)
# ==========================================
# Lệnh này tự động quét hệ thống (Win/Mac/Linux) để tìm SFML
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# Link thư viện chuẩn của SFML. 
# SFML:: sẽ tự động lo hết các thư viện phụ (freetype, opengl, winmm...) bạn không cần gõ tay nữa!
target_link_libraries(SudokuProject PRIVATE SFML::Graphics SFML::Window SFML::System)


# ==========================================
# TỰ ĐỘNG COPY ASSETS VÀO THƯ MỤC BUILD
# ==========================================
add_custom_command(TARGET SudokuProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:SudokuProject>/assets"
)

# ==========================================
# CẤU HÌNH CPACK TẠO FILE CÀI ĐẶT
# ==========================================
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "SudokuProject")
set(CPACK_PACKAGE_VENDOR "HuyPham")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sudoku AI Platform")
set(CPACK_PACKAGE_VERSION "1.0.0")

# Lệnh cài đặt file thực thi và assets vào máy người dùng
install(TARGETS SudokuProject DESTINATION bin)
install(DIRECTORY assets/ DESTINATION bin/assets) 

# Cấu hình định dạng file xuất ra
if(WIN32)
    set(CPACK_GENERATOR "NSIS") # Tạo file .exe cài đặt cho Windows
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop") # Tạo file .dmg cho Mac
else()
    set(CPACK_GENERATOR "DEB") # Tạo file .deb cho Ubuntu
endif()

include(CPack)