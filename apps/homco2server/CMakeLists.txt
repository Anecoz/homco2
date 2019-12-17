file(GLOB_RECURSE src
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(homco2server ${src})
target_include_directories(homco2server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(homco2server cpprestsdk::cpprest)