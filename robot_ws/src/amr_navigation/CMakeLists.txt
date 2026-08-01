cmake_minimum_required(VERSION 3.8)
project(amr_navigation)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_library(
    tf_helper
    src/tf_helper.cpp
)

target_include_directories(
    tf_helper
    PUBLIC
    include
)

ament_target_dependencies(
    tf_helper
    rclcpp
    tf2
    tf2_ros
    tf2_geometry_msgs
)

add_executable(
    navigation_node
    src/navigation_node.cpp
)

target_link_libraries(
    navigation_node
    tf_helper
)

ament_target_dependencies(
    navigation_node
    rclcpp
)

install(
    TARGETS
    navigation_node
    DESTINATION
    lib/${PROJECT_NAME}
)

ament_package()
