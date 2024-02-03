cmake_minimum_required(VERSION 3.10)
project(GetTable)

# Укажите здесь используемую версию C++
set(CMAKE_CXX_STANDARD 17)

# Настройка путей к пакетам, установленным через vcpkg
set(VCPKG_TARGET_TRIPLET x64-windows)
set(CMAKE_TOOLCHAIN_FILE E:/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(VCPKG_INSTALLED_DIR "E:/vcpkg/installed")

set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv4")
set(Protobuf_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/protobuf")
set(TIFF_LIBRARY "${VCPKG_INSTALLED_DIR}/x64-windows/lib/tiff.lib")
set(TIFF_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/include")
set(quirc_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/quirc")
set(Tesseract_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/tesseract")
set(CURL_LIBRARY "${VCPKG_INSTALLED_DIR}/x64-windows/lib/libcurl.lib")
set(CURL_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/include")
set(Leptonica_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/leptonica")
set(LibArchive_LIBRARY "${VCPKG_INSTALLED_DIR}/x64-windows/lib/archive.lib")
set(LibArchive_INCLUDE_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/include")

# Находим и подключаем OpenCV и Tesseract
find_package(OpenCV REQUIRED)
find_package(Tesseract REQUIRED)

# Добавляем исполняемый файл
add_executable(GetTable GetTable.cpp)

# Связываем вашу программу с библиотеками OpenCV и Tesseract
target_link_libraries(GetTable PRIVATE ${OpenCV_LIBS} Tesseract::libtesseract)

# Если есть дополнительные зависимости или пути к заголовочным файлам
# include_directories(путь к дополнительным заголовочным файлам)