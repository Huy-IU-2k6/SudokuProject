cmake_minimum_required(VERSION 3.16)
project(SudokuPlatform VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# ==========================================
# CẤU HÌNH TỰ ĐỘNG LẤY ICON CHO WIN & MAC
# ==========================================
if(WIN32 AND EXISTS "${CMAKE_SOURCE_DIR}/icon.rc")
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/icon.rc")
elseif(APPLE AND EXISTS "${CMAKE_SOURCE_DIR}/icon.icns")
    # Khai báo cho Mac biết file icns là tài nguyên quan trọng
    set_source_files_properties("${CMAKE_SOURCE_DIR}/icon.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/icon.icns")
endif()

# Gắn nhãn MACOSX_BUNDLE để biến game thành ứng dụng Mac chuẩn (.app) thay vì file thực thi đen ngòm
add_executable(SudokuProject MACOSX_BUNDLE ${SOURCES})
target_include_directories(SudokuProject PRIVATE src)

if(APPLE)
    # Cấu hình đính icon vào App Bundle của Mac
    set_target_properties(SudokuProject PROPERTIES MACOSX_BUNDLE_ICON_FILE "icon.icns")
endif()

# ==========================================
# CẤU HÌNH TỰ ĐỘNG TẢI SFML 3.1.0 
# ==========================================
include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)
FetchContent_MakeAvailable(SFML)

target_link_libraries(SudokuProject PRIVATE SFML::Graphics SFML::Window SFML::System)

# ==========================================
# TỰ ĐỘNG COPY ASSETS
# ==========================================
add_custom_command(TARGET SudokuProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:SudokuProject>/assets"
)

# ==========================================
# CẤU HÌNH CPACK - TẠO BỘ CÀI ĐẶT
# ==========================================
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "SudokuProject")
set(CPACK_PACKAGE_VENDOR "HuyPham")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sudoku AI Platform")
set(CPACK_PACKAGE_VERSION "1.0.0")

if(WIN32)
    install(TARGETS SudokuProject DESTINATION .)
    install(DIRECTORY assets/ DESTINATION assets) 
    
    set(CPACK_GENERATOR "NSIS") 
    set(CPACK_PACKAGE_EXECUTABLES "SudokuProject" "Sudoku AI Platform")
    set(CPACK_CREATE_DESKTOP_LINKS "SudokuProject")
    
    if(EXISTS "${CMAKE_SOURCE_DIR}/icon.ico")
        set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/icon.ico")
        set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/icon.ico")
    endif()

elseif(APPLE)
    # Trên Mac, phải cài assets chui vào bên trong ruột của file .app để không bị lỗi Font
    install(TARGETS SudokuProject BUNDLE DESTINATION .)
    install(DIRECTORY assets/ DESTINATION "SudokuProject.app/Contents/MacOS/assets")
    
    set(CPACK_GENERATOR "DragNDrop") 
    
    # Gắn Icon cho cái đĩa cài đặt .dmg của Mac
    if(EXISTS "${CMAKE_SOURCE_DIR}/icon.icns")
        set(CPACK_DMG_VOLUME_ICON "${CMAKE_SOURCE_DIR}/icon.icns")
    endif()
endif()

include(CPack)