include(${CMAKE_SOURCE_DIR}/cmake/module.cmake)

include_directories(${CMAKE_SOURCE_DIR}/Utils)

find_package(Drogon REQUIRED)

add_module(KafkaDataSender DEPENDS Logger LIBS rdkafka)