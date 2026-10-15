#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"

namespace lab1_ns
{

class MyComponent : public rclcpp::Node
{
public:
  explicit MyComponent(const rclcpp::NodeOptions & options)
  : Node("my_component_node", options)
  {
    RCLCPP_INFO(this->get_logger(), "Вузол-компонент MyComponent успішно ініціалізовано!");
  }
};

}  

RCLCPP_COMPONENTS_REGISTER_NODE(lab1_ns::MyComponent)
