#include "amr_navigation/waypoint_navigator.h"


namespace amr_navigation
{
WaypointNavigator::WaypointNavigator(rclcpp::Node::SharedPtr node) : m_node(node)
{
    m_client = rclcpp_action::create_client<FollowWaypoints>(m_node, "follow_waypoints");
}

bool WaypointNavigator::waitForServer()
{
    if(!m_client->wait_for_action_server(std::chrono::seconds(10)))
    {
        RCLCPP_ERROR(m_node->get_logger(), "follow_waypoints action server unavailable");
        return false;
    }

    return true;
}

bool WaypointNavigator::followWaypoints(const std::vector<geometry_msgs::msg::PoseStamped>& poses)
{
    if(!waitForServer())
        return false;
    
    auto goal_msg = FollowWaypoints::Goal();
    goal_msg.poses = poses;
    auto future_goal = m_client->async_send_goal(goal_msg);

    if(rclcpp::spin_until_future_complete(m_node, future_goal) != rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR( m_node->get_logger(), "send waypoint goal failed");
        return false;
    }

    auto goal_handle = future_goal.get();
    if(!goal_handle)
    {
        RCLCPP_ERROR(m_node->get_logger(), "waypoint rejected");
        return false;
    }

    RCLCPP_INFO(m_node->get_logger(), "waypoint accepted");

    auto result_future = m_client->async_get_result(goal_handle);

    while(rclcpp::ok() && result_future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready)
    {
        rclcpp::spin_some(m_node);
    }

    auto result = result_future.get();
    switch(result.code)
    {
        case rclcpp_action::ResultCode::SUCCEEDED:
            RCLCPP_INFO( m_node->get_logger(), "waypoint navigation success");
            return true;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_WARN(m_node->get_logger(), "waypoint canceled");
            return false;
        default:
            RCLCPP_ERROR(m_node->get_logger(), "waypoint failed");
            return false;
    }
}

}