set(JPEG_SPECIFIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/JPEGCore.cpp ${CMAKE_CURRENT_SOURCE_DIR}/JPEGHandler.cpp)

target_sources(cil PUBLIC ${JPEG_SPECIFIC_SOURCES})

get_target_property(cil_target_type cil TYPE)

target_include_directories(cil PUBLIC ${${ACTIVE_JPEG_LIB}_INCLUDE_DIRS})

if(cil_target_type STREQUAL "SHARED_LIBRARY")
  target_link_directories(cil PUBLIC ${${ACTIVE_JPEG_LIB}_LIBRARY_DIRS})
  target_link_libraries(cil PUBLIC ${${ACTIVE_JPEG_LIB}_LIBRARIES})
endif()