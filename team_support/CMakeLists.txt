cmake_minimum_required(VERSION 3.8)
project(team_support)

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

#for a team
add_executable(a_team src/a_team.cpp)

#for b team
add_executable(b_team src/b_team.cpp)
add_executable(joy2vel4b src/joy2vel4b.cpp)

#for a team
target_include_directories(a_team PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

#for b team
target_include_directories(b_team PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(joy2vel4b PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

#for a team 
ament_target_dependencies(
  a_team
  rclcpp
  std_msgs
  geometry_msgs
)

#for b team
ament_target_dependencies(
  b_team
  rclcpp
  std_msgs
  geometry_msgs
)

ament_target_dependencies(
  joy2vel4b
  rclcpp
  geometry_msgs
  sensor_msgs
)

#for a team
install(TARGETS a_team
  DESTINATION lib/${PROJECT_NAME})

#for b team
install(TARGETS b_team
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS joy2vel4b
  DESTINATION lib/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
