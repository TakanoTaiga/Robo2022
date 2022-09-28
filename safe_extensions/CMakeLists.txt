cmake_minimum_required(VERSION 3.8)
project(safe_extensions)

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
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)



add_executable(safeLidar src/safe_lidar.cpp)
add_executable(safeWall src/safe_wall.cpp)

target_include_directories(safeLidar PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(safeWall PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(
  safeLidar
  rclcpp
  sensor_msgs
  std_msgs
)
ament_target_dependencies(
  safeWall
  rclcpp
  geometry_msgs
  std_msgs
)

install(TARGETS safeLidar
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS safeWall
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
