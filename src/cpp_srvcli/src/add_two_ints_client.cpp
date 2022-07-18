#include "rclcpp/rclcpp.hpp"
//#include "example_interfaces/srv/add_two_ints.hpp"
#include "tutorial_interfaces/srv/add_three_ints.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    // Require 1 command + 3 numbers as argument
    if (argc != 4) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "usage: add_three_ints_client X Y Z");
        return 1;
    }

    // Create the ROS2 node
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("add_three_ints_client");

    // Create the client of the Service
    rclcpp::Client<tutorial_interfaces::srv::AddThreeInts>::SharedPtr client =
    node->create_client<tutorial_interfaces::srv::AddThreeInts>("add_three_ints");

    // Create the request data
    auto request = std::make_shared<tutorial_interfaces::srv::AddThreeInts::Request>();
    // int64 type variables
    request->a = atoll(argv[1]);
    request->b = atoll(argv[2]);
    request->c = atoll(argv[2]);

    // Search for the 'service' node that 'client' is searching for.
    while (!client->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
            return 0;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto result = client->async_send_request(request);

    // Wait for the result by spinning the node
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld", result.get()->sum);
    
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_three_ints");
    }

    rclcpp::shutdown();
    return 0;
}