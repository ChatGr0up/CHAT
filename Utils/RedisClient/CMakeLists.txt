include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)
set(HIREDIS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdPartyDepends/include/hiredis")
set(HIREDIS_LIBRARY "${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib/libhiredis.so")

include_directories(${HIREDIS_INCLUDE_DIR})
link_directories("${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib")
# set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/usr/local/redis/redis-plus-plus/share/cmake/redis++")
# find_package(redis++ REQUIRED)

add_module(RedisClient Depends JsonUtils Logger EnvManager LIBS "${HIREDIS_LIBRARY}")