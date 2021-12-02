include_directories(${CMAKE_SOURCE_DIRECTORY}/include)
link_directories(${CMAKE_BINARY_DIR}/lib)
link_libraries(cil)

if(BUILD_SHARED_LIBS)
    list(APPEND CMAKE_BUILD_RPATH ${CMAKE_BINARY_DIR}/lib)
    list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_BINARY_DIR}/lib)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING REGEX ".*\.(jpeg|jpg|png)")

if (CIL_ENABLE_PNG)
    list(APPEND CMAKE_BUILD_RPATH ${LIBPNG_LIBRARY_DIRS})
    list(APPEND CMAKE_INSTALL_RPATH ${LIBPNG_LIBRARY_DIRS})
    if (NOT BUILD_SHARED_LIBS)
        link_directories(${LIBPNG_LIBRARY_DIRS})
        link_libraries(${LIBPNG_LIBRARIES})
    endif()
endif()
    
if (CIL_ENABLE_JPEG)
    list(APPEND CMAKE_BUILD_RPATH ${${ACTIVE_JPEG_LIB}_DIRS})
    list(APPEND CMAKE_INSTALL_RPATH ${${ACTIVE_JPEG_LIB}_LIBRARY_DIRS})
    if (NOT BUILD_SHARED_LIBS)
        link_directories(${${ACTIVE_JPEG_LIB}_LIBRARY_DIRS})
        link_libraries(${${ACTIVE_JPEG_LIB}_LIBRARIES})
    endif()
endif()

if (CIL_ENABLE_PNG)
    add_subdirectory(PNG)
endif()

if (CIL_ENABLE_JPEG)
    add_subdirectory(JPEG)
endif()
