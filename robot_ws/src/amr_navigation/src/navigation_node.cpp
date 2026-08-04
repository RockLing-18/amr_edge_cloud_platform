#include "rclcpp/rclcpp.hpp"
#include "amr_navigation/tf_helper.h"
#include "amr_navigation/navigation_manager.h"
#include "amr_navigation/waypoint_navigator.h"


class NavigationNode : public rclcpp::Node
{
public:
    NavigationNode() : Node("navigation_node",
        rclcpp::NodeOptions()
            .append_parameter_override(
                "use_sim_time",
                true)
    )
    {
    }

    void init()
    {
        m_tf_helper = std::make_shared<amr_navigation::TFHelper>(shared_from_this());
        m_navigation_manager = std::make_shared<amr_navigation::NavigationManager>(shared_from_this());
        m_navigator = std::make_shared<amr_navigation::WaypointNavigator>(shared_from_this());
        // 测试导航
        //sendNavigationGoal();

        // 测试多点导航
        sendWaypointGoal();

        m_timer = create_wall_timer(
                std::chrono::seconds(1),
                std::bind(&NavigationNode::updatePose, this)
            );
    }

private:
    void updatePose()
    {
        amr_navigation::RobotPose pose;
        if(m_tf_helper->getRobotPose(pose))
        {
            RCLCPP_INFO(get_logger(), 
                "Robot pose x=%.2f y=%.2f yaw=%.2f",
                pose.x,
                pose.y,
                pose.yaw
            );
        }
    }

    void sendNavigationGoal()
    {
        geometry_msgs::msg::PoseStamped goal;
        goal.header.frame_id="map";
        goal.header.stamp = this->now();
        goal.pose.position.x=2.0;
        goal.pose.position.y=2.0;
        // yaw=0
        goal.pose.orientation.w=1.0;
        m_navigation_manager->navigateTo(goal);
    }


    void sendWaypointGoal()
    {
        std::vector<geometry_msgs::msg::PoseStamped> goals;

        auto create_goal = [&](double x,double y)
        {
            geometry_msgs::msg::PoseStamped pose;
            pose.header.frame_id="map";
            pose.header.stamp = this->now();
            pose.pose.position.x=x;
            pose.pose.position.y=y;
            pose.pose.orientation.w=1.0;
            return pose;
        };

        goals.push_back(create_goal(0,0));
        goals.push_back(create_goal(2,0));
        goals.push_back(create_goal(2,2));
        m_navigator->followWaypoints(goals);
    }
private:
    std::shared_ptr<amr_navigation::TFHelper> m_tf_helper;
    std::shared_ptr<amr_navigation::NavigationManager> m_navigation_manager;
    std::shared_ptr<amr_navigation::WaypointNavigator> m_navigator;
    rclcpp::TimerBase::SharedPtr m_timer;
};



int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NavigationNode>();
    node->init();
    rclcpp::spin(node);
    rclcpp::shutdown();
}