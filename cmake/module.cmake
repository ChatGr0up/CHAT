function(add_module MODULE_NAME)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(MODULE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # 输出调试信息
    message(STATUS "Searching for source files in ${CMAKE_SOURCE_DIR}/modules/${MODULE_NAME}")

    # 获取当前模块目录下的所有 .cpp 文件
    file(GLOB MODULE_SOURCES ${CMAKE_SOURCE_DIR}/modules/${MODULE_NAME}/*.cpp)

    message(STATUS "Found source files for ${MODULE_NAME}: ${MODULE_SOURCES}")

    if (NOT MODULE_SOURCES)
        message(FATAL_ERROR "No source files found for module ${MODULE_NAME}")
    endif()

    # 创建共享库
    add_library(${MODULE_NAME} SHARED ${MODULE_SOURCES})

    # 设置目标的包含目录
    target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/modules/${MODULE_NAME})

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
endfunction()
