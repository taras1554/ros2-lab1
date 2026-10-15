#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class ManagedNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit ManagedNode(const std::string & node_name)
  : rclcpp_lifecycle::LifecycleNode(node_name)
  {
    RCLCPP_INFO(get_logger(), "[%s] створено (стан: Unconfigured).", node_name.c_str());
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] on_configure(): Конфігурація...", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State & state) override
  {
    LifecycleNode::on_activate(state);
    RCLCPP_INFO(get_logger(), "[%s] on_activate(): Вузол АКТИВОВАНО!", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & state) override
  {
    LifecycleNode::on_deactivate(state);
    RCLCPP_INFO(get_logger(), "[%s] on_deactivate(): Деактивовано.", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] on_cleanup(): Очищення ресурсів.", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] on_shutdown(): Завершення роботи.", get_name());
    return CallbackReturn::SUCCESS;
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor executor;

  auto node_1 = std::make_shared<ManagedNode>("managed_node_1");
  auto node_2 = std::make_shared<ManagedNode>("managed_node_2");

  executor.add_node(node_1->get_node_base_interface());
  executor.add_node(node_2->get_node_base_interface());

  executor.spin();
  rclcpp::shutdown();
  return 0;
}
