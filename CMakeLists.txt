cmake_minimum_required(VERSION 3.16)
project(SudokuPlatform VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(SudokuProject ${SOURCES})

target_include_directories(SudokuProject PRIVATE src)

# ==========================================
# CẤU HÌNH TỰ ĐỘNG TẢI SFML 3.1.0 (FETCH CONTENT)
# ==========================================
include(FetchContent)

# Yêu cầu tải bản Static
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# TẮT MODULE MẠNG VÀ ÂM THANH ĐỂ TRÁNH LỖI MbedTLS TRÊN LINUX VÀ TĂNG TỐC BUILD!
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)
FetchContent_MakeAvailable(SFML)

# Link thư viện SFML (Chỉ cần Graphics và Window)
target_link_libraries(SudokuProject PRIVATE SFML::Graphics SFML::Window SFML::System)

# ==========================================
# TỰ ĐỘNG COPY ASSETS VÀ CPACK CÀI ĐẶT
# ==========================================
add_custom_command(TARGET SudokuProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:SudokuProject>/assets"
)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "SudokuProject")
set(CPACK_PACKAGE_VENDOR "HuyPham")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sudoku AI Platform")
set(CPACK_PACKAGE_VERSION "1.0.0")

install(TARGETS SudokuProject DESTINATION bin)
install(DIRECTORY assets/ DESTINATION bin/assets) 

if(WIN32)
    set(CPACK_GENERATOR "NSIS") 
    # --- 2 DÒNG MỚI ĐỂ TẠO ICON START MENU VÀ DESKTOP SHORTCUT ---
    # Cú pháp: set(CPACK_PACKAGE_EXECUTABLES "Tên_Target_Của_Bạn" "Tên_Hiển_Thị_Ra_Màn_Hình")
    set(CPACK_PACKAGE_EXECUTABLES "SudokuProject" "Sudoku AI Platform")
    
    # Ra lệnh cho NSIS tự động tạo lối tắt (Shortcut) ngoài Desktop
    set(CPACK_CREATE_DESKTOP_LINKS "SudokuProject")
    # -------------------------------------------------------------
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop") 
else()
    set(CPACK_GENERATOR "DEB") 
endif()

include(CPack)