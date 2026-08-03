#pragma once
#include <memory>
#include <atomic>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

namespace amr_navigation
{

class NavigationManager
{
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandle = rclcpp_action::ClientGoalHandle<NavigateToPose>;
    explicit NavigationManager(rclcpp::Node::SharedPtr node);


    /**
     * 发送导航目标
     */
    bool navigateTo(const geometry_msgs::msg::PoseStamped &goal_pose);

    /**
     * 取消当前导航
     */
    void cancelNavigation();

    /**
     * 是否正在导航
     */
    bool isNavigating() const;

private:
    void goalResponseCallback(GoalHandle::SharedPtr goal_handle);
    void feedbackCallback(GoalHandle::SharedPtr, const std::shared_ptr<const NavigateToPose::Feedback> feedback);
    void resultCallback(const GoalHandle::WrappedResult &result);

private:
    rclcpp::Node::SharedPtr m_node;
    rclcpp_action::Client<NavigateToPose>::SharedPtr m_action_client;
    GoalHandle::SharedPtr m_goal_handle;
    std::atomic<bool> m_navigating{false};
};

}