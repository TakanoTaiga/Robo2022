cmake_minimum_required(VERSION 3.8)
project(robo2022_test)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_FLAGS "-O2")
  set(CMAKE_CXX_FLAGS "-mtune=native")
  set(CMAKE_CXX_FLAGS "-march=native")
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(vecMonitor src/vec_monitor.cpp)
add_executable(debug src/debug.cpp)

#test tools
target_include_directories(vecMonitor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(debug PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

#test tools
ament_target_dependencies(
  vecMonitor
  rclcpp
  geometry_msgs
)

ament_target_dependencies(
  debug
  rclcpp
  std_msgs
)

#test tools
install(TARGETS vecMonitor
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS debug
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
