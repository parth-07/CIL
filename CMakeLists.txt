cmake_minimum_required(VERSION 3.16)

project(CIL
       VERSION 0.1.0
       LANGUAGES C CXX
       DESCRIPTION "C++ Image Library")

find_package(PkgConfig REQUIRED)

option(LIBPNG_DIR "Path to libpng installation directory")
option(LIBJPEG_DIR "Path to libjpeg installation directory")
option(CIL_BUILD_EXAMPLES "Specify if examples should be added and built")

if(ENV{PKG_CONFIG_PATH})
    set(pkg_config_path $ENV{PKG_CONFIG_PATH})
    string(REPLACE ":" ";" prefix_paths ${pkg_config_path})
    list(APPEND CMAKE_PREFIX_PATH ${prefix_paths})
endif()

if(LIBPNG_DIR)
    list(PREPEND CMAKE_PREFIX_PATH ${LIBPNG_DIR})
endif()
    
if(LIBJPEG_DIR)
    list(PREPEND CMAKE_PREFIX_PATH ${LIBJPEG_DIR})
endif()
    
pkg_check_modules(LIBPNG libpng=1.6.37)
if(LIBPNG_FOUND)
    set(CIL_ENABLE_PNG true)
    string(CONCAT libpng_found_message "Libpng ${LIBPNG_VERSION} will "
           "be used to add PNG support to the project.")
    message(STATUS ${libpng_found_message})
else()
    set(CIL_ENABLE_PNG false)
    if (LIBPNG_DIR)
        string(CONCAT libpng_not_found_warning "Libpng pkg config file not found. "
               "Project is being configured to be built without the PNG support.")
        message(WARNING ${libpng_not_found_warning})
    endif()
endif()

pkg_check_modules(LIBJPEG libjpeg=9.4.0)
if(LIBJPEG_FOUND)
    set(CIL_ENABLE_JPEG true)
    string(CONCAT libjpeg_found_message "Libjpeg ${LIBJPEG_VERSION} will "
           "be used to add JPEG support to the project.")
    message(STATUS ${libjpeg_found_message})
else()
    set(CIL_ENABLE_JPEG false)
    if (LIBJPEG_DIR)
        string(CONCAT libjpeg_not_found_warning "Libjpeg pkg config file not found. "
               "Project is being configured to be built without the JPEG support.")
        message(WARNING ${libjpeg_not_found_warning})
    endif()
endif()

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_subdirectory(lib)

if(CIL_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(DIRECTORY include/ TYPE INCLUDE)