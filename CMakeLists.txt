cmake_minimum_required(VERSION 3.15)
project(SudokuPlatform VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tự động quét toàn bộ file .cpp trong src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(SudokuProject ${SOURCES})

# Thư mục include nội bộ của dự án và SFML
target_include_directories(SudokuProject PRIVATE 
    src 
    "D:/SFML/SFML-3.1.0-windows-gcc-14.2.0-mingw-64-bit/SFML-3.1.0/include"
)

# Chỉ đường dẫn cho Linker mò vào thư mục chứa file .a của bạn
target_link_directories(SudokuProject PRIVATE
    "D:/SFML/SFML-3.1.0-windows-gcc-14.2.0-mingw-64-bit/SFML-3.1.0/lib"
)

# Bật định nghĩa Macro STATIC cho Compiler biết
target_compile_definitions(SudokuProject PRIVATE SFML_STATIC)

# ĐƯỜNG LỐI MỚI: Bỏ hoàn toàn find_package(Freetype) và find_package(SFML) 
# vì chúng phá hỏng cấu trúc do lỗi file cấu hình đi kèm gói tải về.
# Chúng ta sẽ link trực tiếp bằng tên file tĩnh (.a) có sẵn trong folder lib của bạn.

target_link_libraries(SudokuProject PRIVATE
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    freetype           # Thằng này gọi HarfBuzz
    harfbuzz           # FIX: Thêm HarfBuzz vào đây để giải quyết các lỗi hb_
    opengl32
    winmm
    gdi32
    user32
)
# Tự động copy thư mục assets vào chung chỗ với file .exe khi build
add_custom_command(TARGET SudokuProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:SudokuProject>/assets"
)