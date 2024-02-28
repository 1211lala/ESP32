# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/SoftWare/ESP_IDF/esp_idf_v_5.1.1/esp-idf/components/bootloader/subproject"
  "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader"
  "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix"
  "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix/tmp"
  "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix/src/bootloader-stamp"
  "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix/src"
  "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/MY_CODE/ESP32/IDF/9_JSON/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
