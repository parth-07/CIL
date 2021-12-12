set(PNG_SPECIFIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/PNGCore.cpp ${CMAKE_CURRENT_SOURCE_DIR}/PNGHandler.cpp)

target_sources(cil PRIVATE ${PNG_SPECIFIC_SOURCES})

target_include_directories(cil PRIVATE ${LIBPNG_INCLUDE_DIRS})

target_link_libraries(cil PRIVATE ${LIBPNG_LINK_LIBRARIES})