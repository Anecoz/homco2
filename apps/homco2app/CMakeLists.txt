file(GLOB_RECURSE src
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.qrc
)

add_executable(homco2app ${src})
target_include_directories(homco2app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_link_libraries(homco2app PRIVATE cpprestsdk::cpprest Qt5::Core Qt5::Quick Qt5::Widgets)