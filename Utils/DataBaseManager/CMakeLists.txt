include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)
set(MYSQL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/thirdPartyDepends/include/mysql")
set(MYSQL_LIBRARY "${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib/libmysqlclient.so")
set(MYSQL_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdPartyDepends/lib/libmysqlclient.so")
include_directories(${MYSQL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/Utils)

add_module(DataBaseManager LIBS "${MYSQL_LIBRARIES}")