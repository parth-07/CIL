if(BUILD_SHARED_LIBS)
    list(APPEND CMAKE_BUILD_RPATH ${CMAKE_BINARY_DIR}/lib)
    list(APPEND CMAKE_INSTALL_RPATH ${CMAKE_BINARY_DIR}/lib)
endif()

if (CIL_ENABLE_PNG)
    list(APPEND CMAKE_BUILD_RPATH ${LIBPNG_LIBRARY_DIRS})
    list(APPEND CMAKE_INSTALL_RPATH ${LIBPNG_LIBRARY_DIRS})
endif()
    
if (CIL_ENABLE_JPEG)
    list(APPEND CMAKE_BUILD_RPATH ${${ACTIVE_JPEG_LIB}_DIRS})
    list(APPEND CMAKE_INSTALL_RPATH ${${ACTIVE_JPEG_LIB}_LIBRARY_DIRS})
endif()

file (GLOB_RECURSE
      images
      LIST_DIRECTORIES false
      CONFIGURE_DEPENDS
      *.jpeg *.jpg *.png *.ppm)

set(test_image_files)
foreach(image IN LISTS images)
    file(RELATIVE_PATH image_rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${image})
    set(output_image ${CMAKE_CURRENT_BINARY_DIR}/${image_rel_path})
    add_custom_command(OUTPUT ${output_image}
                       COMMAND ${CMAKE_COMMAND} -E copy ${image} ${output_image}
                       DEPENDS ${image}
                       VERBATIM)
    list(APPEND test_image_files ${output_image})
endforeach()

add_custom_target(copy-test-images ALL DEPENDS ${test_image_files})

include_directories(${CATCH2_INCLUDE_DIRS})

add_library(catch2_main OBJECT main.cpp)

file(GLOB
     test_files
     LIST_DIRECTORIES false
     CONFIGURE_DEPENDS
     *.cpp)

list(REMOVE_ITEM test_files ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

foreach(test_file IN LISTS test_files)
  get_filename_component(test_name ${test_file} NAME_WE)
  STRING(CONCAT test ${test_name} "-test")
  add_executable(${test} ${test_file})
  set_target_properties(${test} PROPERTIES OUTPUT_NAME ${test_name})
  target_link_libraries(${test} PRIVATE cil catch2_main)
  add_test(NAME ${test} COMMAND ${test})
endforeach()
