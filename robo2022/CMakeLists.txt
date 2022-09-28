cmake_minimum_required(VERSION 3.5)
project(robo2022)

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
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# find cpp file
add_executable(joy2vel src/joy2vel.cpp)
add_executable(robo2022 src/robo2022.cpp)
add_executable(mdc2022Connect src/mdc2022Connect.cpp)
add_executable(motion_smoother src/motion_smoother.cpp)
add_executable(errorChecker src/errorChecker.cpp)


target_include_directories(joy2vel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(robo2022 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(mdc2022Connect PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(motion_smoother PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(errorChecker PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)



ament_target_dependencies(
  joy2vel
  rclcpp
  geometry_msgs
  sensor_msgs
)

ament_target_dependencies(
  robo2022
  rclcpp
  geometry_msgs
  std_msgs
)

ament_target_dependencies(
  mdc2022Connect
  rclcpp
  std_msgs
)

ament_target_dependencies(
  motion_smoother
  rclcpp
  geometry_msgs
)

ament_target_dependencies(
  errorChecker
  rclcpp
  std_msgs
)



install(TARGETS joy2vel
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS robo2022
  DESTINATION lib/${PROJECT_NAME})
  
install(TARGETS mdc2022Connect
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS motion_smoother
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS errorChecker
  DESTINATION lib/${PROJECT_NAME})



if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
