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
# FIX LINUX: Chuyển các chữ cái thành CHỮ THƯỜNG (graphics window system)
find_package(SFML COMPONENTS graphics window system REQUIRED)

# Link thư viện chuẩn của SFML. 
target_link_libraries(SudokuProject PRIVATE sfml-graphics sfml-window sfml-system)

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

install(TARGETS SudokuProject DESTINATION bin)
install(DIRECTORY assets/ DESTINATION bin/assets) 

if(WIN32)
    set(CPACK_GENERATOR "NSIS") 
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop") 
else()
    set(CPACK_GENERATOR "DEB") 
endif()

include(CPack)