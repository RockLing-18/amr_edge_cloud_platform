#include "rclcpp/rclcpp.hpp"
#include "amr_navigation/tf_helper.h"


class NavigationNode : public rclcpp::Node
{
public:
    NavigationNode() : Node("navigation_node")
    {
    }

    void init()
    {
        m_tf_helper = std::make_shared<amr_navigation::TFHelper>(shared_from_this());
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

private:
    std::shared_ptr<amr_navigation::TFHelper> m_tf_helper;
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