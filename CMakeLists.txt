cmake_minimum_required(VERSION 3.22)
project(ethereal VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Экспорт compile_commands.json для LSP (clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Главный бинарник
add_executable(ethereal src/main.cpp)

# Подключение заголовочных файлов
target_include_directories(ethereal PRIVATE include)

# Строгие проверки компилятора
target_compile_options(ethereal PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
