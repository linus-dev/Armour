include_directories ("${PROJECT_SOURCE_DIR}/include/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.68.0 REQUIRED COMPONENTS system filesystem date_time)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(Armour_Server server.cpp)
    target_link_libraries(Armour_Server pthread ${Boost_LIBRARIES})
endif()

