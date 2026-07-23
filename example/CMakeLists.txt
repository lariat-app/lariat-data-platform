find_package(Libevent CONFIG REQUIRED)

add_library(example_service SHARED ${CMAKE_CURRENT_SOURCE_DIR}/service.cpp)
target_link_libraries(example_service PRIVATE ldpcore libevent::core libevent::extra yyjson::yyjson spdlog::spdlog)

set_target_properties(example_service PROPERTIES
    PREFIX "lib"
)

add_library(example_fifo SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugin.cpp)
target_link_libraries(example_fifo PRIVATE ldpcore)

set_target_properties(example_fifo PROPERTIES
    PREFIX "libldp_"
)