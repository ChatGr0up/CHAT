function(add_module MODULE_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs DEPENDS LIBS)
    cmake_parse_arguments(MODULE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 输出调试信息，显示当前目录路径
    message(STATUS "Searching for source files in ${CMAKE_CURRENT_LIST_DIR}/")

    # 获取当前模块目录下的所有 .cpp 文件
    file(GLOB MODULE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

    message(STATUS "Found source files for ${MODULE_NAME}: ${MODULE_SOURCES}")

    if (NOT MODULE_SOURCES)
        message(FATAL_ERROR "No source files found for module ${MODULE_NAME}")
    endif()

    # 创建共享库
    add_library(${MODULE_NAME} SHARED ${MODULE_SOURCES})

    # 设置目标的包含目录
    target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

    # 设置输出目录
    set_target_properties(${MODULE_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # 处理依赖关系
    if (MODULE_DEPENDS)
        foreach(DEP ${MODULE_DEPENDS})
            message(STATUS "Module ${MODULE_NAME} depends on ${DEP}")
            add_dependencies(${MODULE_NAME} ${DEP})
            target_link_libraries(${MODULE_NAME} PRIVATE ${DEP})
        endforeach()
    endif()

    # 处理外部库依赖
    if (MODULE_LIBS)
        foreach(LIB ${MODULE_LIBS})
            message(STATUS "Module ${MODULE_NAME} links to external library: ${LIB}")
            target_link_libraries(${MODULE_NAME} PUBLIC ${LIB})
        endforeach()
    endif()
endfunction()
