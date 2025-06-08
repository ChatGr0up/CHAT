include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)
include_directories(/usr/local/redis/redis-plus-plus/include)
include_directories(/usr/local/redis/hiredis/include)

link_directories(/usr/local/redis/hiredis/lib)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/usr/local/redis/redis-plus-plus/share/cmake/redis++")
find_package(redis++ REQUIRED)

add_module(RedisClient Depends JsonUtils Logger EnvManager LIBS redis++::redis++ hiredis)