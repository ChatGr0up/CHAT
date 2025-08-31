include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/thirdPartyDepends;${CMAKE_PREFIX_PATH}")
include_directories(${CMAKE_SOURCE_DIR}/thirdPartyDepends/include)
set(MYSQL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdPartyDepends/include/mysql" CACHE PATH "MySQL include dir")
set(MYSQL_LIBRARY "${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib/libmysqlclient.so" CACHE FILEPATH "MySQL client library")
set(DROGON_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib/libdrogon.so")

include_directories(${CMAKE_SOURCE_DIR}/Utils)
include_directories(${MYSQL_INCLUDE_DIR})
set(MYSQL_LIBRARIES ${MYSQL_LIBRARY} CACHE FILEPATH "MySQL libraries")
find_package(Drogon REQUIRED)
link_directories(${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib)

add_module(RestFrame Depends JsonUtils Logger EnvManager LIBS ${DROGON_LIBRARIES} hiredis ${MYSQL_LIBRARIES})