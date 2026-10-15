cmake_minimum_required(VERSION 3.8)
project(lab1_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)

# Завдання 1.2: Спільна бібліотека компонента
add_library(my_component SHARED src/my_component.cpp)
ament_target_dependencies(my_component rclcpp rclcpp_components)
rclcpp_components_register_nodes(my_component "lab1_ns::MyComponent")

# Завдання 1.3: Керовані вузли та супервізор
add_executable(managed_nodes_exec src/managed_node.cpp)
ament_target_dependencies(managed_nodes_exec rclcpp rclcpp_lifecycle lifecycle_msgs)

add_executable(lifecycle_supervisor src/lifecycle_supervisor.cpp)
ament_target_dependencies(lifecycle_supervisor rclcpp lifecycle_msgs)

install(TARGETS my_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(TARGETS managed_nodes_exec lifecycle_supervisor
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
