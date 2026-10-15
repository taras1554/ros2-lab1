#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"

using namespace std::chrono_literals;

class LifecycleSupervisor : public rclcpp::Node
{
public:
  LifecycleSupervisor() : Node("lifecycle_supervisor") {}

  bool change_state(const std::string & node_name, std::uint8_t transition_id)
  {
    std::string service_name = "/" + node_name + "/change_state";
    auto client = this->create_client<lifecycle_msgs::srv::ChangeState>(service_name);

    if (!client->wait_for_service(5s)) {
      RCLCPP_ERROR(get_logger(), "Сервіс %s недоступний!", service_name.c_str());
      return false;
    }

    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id = transition_id;

    auto future = client->async_send_request(request);
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
      return future.get()->success;
    }
    return false;
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto supervisor = std::make_shared<LifecycleSupervisor>();

  std::vector<std::string> nodes = {"managed_node_1", "managed_node_2"};

  for (const auto & name : nodes) {
    RCLCPP_INFO(supervisor->get_logger(), "Конфігурація %s...", name.c_str());
    supervisor->change_state(name, lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
  }

  for (const auto & name : nodes) {
    RCLCPP_INFO(supervisor->get_logger(), "Активація %s...", name.c_str());
    supervisor->change_state(name, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);
  }

  rclcpp::shutdown();
  return 0;
}
