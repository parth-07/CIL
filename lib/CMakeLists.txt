add_library(cil ImageInfo.cpp ImageMatrix.cpp Pixel.cpp Transformations.cpp)

target_include_directories(cil PUBLIC ${CMAKE_SOURCE_DIR}/include)

if (CIL_ENABLE_JPEG)
    target_compile_definitions(cil PRIVATE CIL_JPEG_ENABLED)
endif()

if (CIL_ENABLE_PNG)
    target_compile_definitions(cil PRIVATE CIL_PNG_ENABLED)
endif()

add_subdirectory(PPM)

if(CIL_ENABLE_PNG)
    add_subdirectory(PNG)
endif()

if(CIL_ENABLE_JPEG)
    add_subdirectory(JPEG)
endif()

add_subdirectory(Core)

install(TARGETS cil 
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)