set(PNG_SPECIFIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/PNGCore.cpp ${CMAKE_CURRENT_SOURCE_DIR}/PNGHandler.cpp)

target_sources(cil PUBLIC ${PNG_SPECIFIC_SOURCES})

get_target_property(cil_target_type cil TYPE)

target_include_directories(cil PUBLIC ${LIBPNG_INCLUDE_DIRS})

if(cil_target_type STREQUAL "SHARED_LIBRARY")
  target_link_directories(cil PUBLIC ${LIBPNG_LIBRARY_DIRS})
  target_link_libraries(cil PUBLIC ${LIBPNG_LIBRARIES})
endif()