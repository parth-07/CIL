file(GLOB 
     example_files
     LIST_DIRECTORIES false
     CONFIGURE_DEPENDS
     *.cpp)

foreach(example_file IN LISTS example_files)
    get_filename_component(example ${example_file} NAME_WE)
    add_executable(${example} ${example_file})
    install(TARGETS ${example} 
            DESTINATION examples/PNG)
endforeach()
