link_libraries(cil)

if(BUILD_SHARED_LIBS)
    list(APPEND CMAKE_BUILD_RPATH ${CMAKE_BINARY_DIR}/lib)
    list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_BINARY_DIR}/lib)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING REGEX ".*\.(jpeg|jpg|png|ppm)")

if (CIL_ENABLE_PNG)
    list(APPEND CMAKE_BUILD_RPATH ${LIBPNG_LIBRARY_DIRS})
    list(APPEND CMAKE_INSTALL_RPATH ${LIBPNG_LIBRARY_DIRS})
endif()
    
if (CIL_ENABLE_JPEG)
    list(APPEND CMAKE_BUILD_RPATH ${${ACTIVE_JPEG_LIB}_DIRS})
    list(APPEND CMAKE_INSTALL_RPATH ${${ACTIVE_JPEG_LIB}_LIBRARY_DIRS})
endif()

file(GLOB
     example_files
     LIST_DIRECTORIES false
     CONFIGURE_DEPENDS
     *.cpp)

foreach(example_file IN LISTS example_files)
    get_filename_component(example ${example_file} NAME_WE)
    add_executable(${example} ${example_file})
    install(TARGETS ${example}
            DESTINATION examples)
endforeach()
