#pragma once
#include <rclcpp/rclcpp.hpp>
#include <nav2_msgs/action/follow_waypoints.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

namespace amr_navigation
{
class WaypointNavigator
{
public:
    using FollowWaypoints = nav2_msgs::action::FollowWaypoints;
    using GoalHandle = rclcpp_action::ClientGoalHandle<FollowWaypoints>;

public:
    explicit WaypointNavigator(rclcpp::Node::SharedPtr node);
    bool waitForServer();
    bool followWaypoints(const std::vector<geometry_msgs::msg::PoseStamped>& poses);

private:
    rclcpp::Node::SharedPtr m_node;
    rclcpp_action::Client<FollowWaypoints>::SharedPtr m_client;
};

}
