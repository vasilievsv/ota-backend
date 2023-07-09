set(FILE_LIST
    template_1
    template_2
    template_3)

foreach(fname ${FILE_LIST})

    add_executable(test_${fname} test_${fname}.cpp)

    target_link_libraries( test_${fname} common )

    target_include_directories(test_${fname}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libs>)

    add_test(NAME test_${fname} COMMAND $<TARGET_FILE:test_${fname}>)

endforeach()
