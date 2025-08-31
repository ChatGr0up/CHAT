include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)
include_directories(${CMAKE_SOURCE_DIR}/Utils)
set(DROGON_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib/libdrogon.so")
add_executable(chat main.cpp)
set_target_properties(chat PROPERTIES
    BUILD_RPATH "${CMAKE_SOURCE_DIR}/build/lib:${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib"
    INSTALL_RPATH "${CMAKE_SOURCE_DIR}/build/lib:${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)
target_link_libraries(chat dl Module JsonUtils EnvManager Logger Thread RestFrame ${DROGON_LIBRARIES})