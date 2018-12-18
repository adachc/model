cmake_minimum_required(VERSION 3.5)

find_package(Boost 1.58.0)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DBOOST_LOG_DYN_LINK -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DBOOST_LOG_DYN_LINK")

# Find source files
FILE(GLOB SOURCES src/*.cpp)
FILE(GLOB_RECURSE INC_ALL "incl/*.h")

# Include header files
include_directories(incl ../incl)

project(ModelGenerator)
# Create an executable
add_executable (${PROJECT_NAME} ${SOURCES} ${INC_ALL})

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
  boost_system
  rt
  pthread
  boost_thread
  boost_filesystem
  boost_log
  boost_log_setup
  boost_date_time
  boost_program_options
  )
