#include "amr_navigation/tf_helper.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"

namespace amr_navigation
{

TFHelper::TFHelper(rclcpp::Node::SharedPtr node): m_node(node)
{
    m_buffer = std::make_unique<tf2_ros::Buffer>(m_node->get_clock());
    m_listener = std::make_shared<tf2_ros::TransformListener>(*m_buffer);
}

bool TFHelper::getRobotPose(RobotPose &pose)
{
    try
    {
        auto transform = m_buffer->lookupTransform(
                "map",
                "base_footprint",
                tf2::TimePointZero
            );

        pose.x = transform.transform.translation.x;
        pose.y = transform.transform.translation.y;

        auto q = transform.transform.rotation;
        tf2::Quaternion quat;
        tf2::fromMsg(q, quat);

        double roll;
        double pitch;

        tf2::Matrix3x3(quat).getRPY(
                roll,
                pitch,
                pose.yaw
            );

        return true;
    }
    catch(const tf2::TransformException &ex)
    {
        RCLCPP_WARN(m_node->get_logger(), "TF error:%s", ex.what());
        return false;
    }

    return false;
}

}