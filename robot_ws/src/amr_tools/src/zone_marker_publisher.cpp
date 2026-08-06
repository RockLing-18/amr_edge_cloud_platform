#include "zone_marker_publisher.h"

ZoneMarkerPublisher::ZoneMarkerPublisher(rclcpp::Node::SharedPtr node) : m_node(node)
{
    m_pub = m_node->create_publisher<visualization_msgs::msg::MarkerArray>("/zone_marker", 10);
}

void ZoneMarkerPublisher::publishZones(const std::vector<Zone>& vZone)
{
    clear();
    if(vZone.empty())
        return;

    visualization_msgs::msg::MarkerArray array;
    for(const auto& zone : vZone)
    {
        array.markers.push_back(createLineMarker(zone));
        array.markers.push_back(createTextMarker(zone));
    }

    m_pub->publish(array);
}

void ZoneMarkerPublisher::publishZone(const Zone &zone)
{
    visualization_msgs::msg::MarkerArray array;
    array.markers.push_back(createLineMarker(zone));
    array.markers.push_back(createTextMarker(zone));
    m_pub->publish(array);
}

void ZoneMarkerPublisher::publishPreview(const Zone& zone)
{
    visualization_msgs::msg::MarkerArray array;
    array.markers.push_back(createLineMarker(zone, true));
    array.markers.push_back(createTextMarker(zone, true));
    m_pub->publish(array);
}

visualization_msgs::msg::Marker ZoneMarkerPublisher::createLineMarker(const Zone& zone, bool preview)
{
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = zone.frame_id;
    marker.header.stamp = m_node->get_clock()->now();
    marker.ns = preview ? "preview_zone_line" : "zone_line";
    marker.id = preview ? 0 : zone.marker_id;
    marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.scale.x = 0.05;

    // 颜色
    marker.color.r = 0.0;
    marker.color.g = 1.0;
    marker.color.b = 0.0;
    marker.color.a = 1.0;

    for(auto& p : zone.polygon)
    {
        geometry_msgs::msg::Point point;
        point.x = p.first;
        point.y = p.second;
        point.z = 0.05;
        marker.points.push_back(point);
    }

    // 闭合多边形
    if(!zone.polygon.empty())
    {
        geometry_msgs::msg::Point first;
        first.x = zone.polygon.front().first;
        first.y = zone.polygon.front().second;
        first.z = 0.05;
        marker.points.push_back(first);
    }

    return marker;
}

visualization_msgs::msg::Marker ZoneMarkerPublisher::createTextMarker(const Zone& zone, bool preview)
{
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = zone.frame_id;
    marker.header.stamp = m_node->get_clock()->now();
    marker.ns = preview ? "preview_zone_text" : "zone_text";
    marker.id = preview ? 0 : zone.marker_id;
    marker.type = visualization_msgs::msg::Marker::TEXT_VIEW_FACING;
    marker.action = visualization_msgs::msg::Marker::ADD;

    /*
        文字位置
        简单取polygon中心点
    */
    double x = 0;
    double y = 0;
    for(auto& p:zone.polygon)
    {
        x += p.first;
        y += p.second;
    }

    if(!zone.polygon.empty())
    {
        x /= zone.polygon.size();
        y /= zone.polygon.size();
    }

    marker.pose.position.x = x;
    marker.pose.position.y = y;
    marker.pose.position.z = 0.2;
    marker.scale.z = 0.3;
    marker.color.r = 1.0;
    marker.color.g = 1.0;
    marker.color.b = 1.0;
    marker.color.a = 1.0;
    marker.text = zone.info.name;
    return marker;
}

void ZoneMarkerPublisher::clearPreview()
{
    visualization_msgs::msg::MarkerArray array;
    visualization_msgs::msg::Marker markerLine;
    markerLine.header.frame_id = "map";
    markerLine.header.stamp = m_node->get_clock()->now();
    markerLine.ns = "preview_zone_line";
    markerLine.id = 0;
    markerLine.action = visualization_msgs::msg::Marker::DELETE;
    array.markers.push_back(markerLine);

    visualization_msgs::msg::Marker markerText = markerLine;
    markerText.ns = "preview_zone_text";
    array.markers.push_back(markerText);
    m_pub->publish(array);
}

void ZoneMarkerPublisher::clear()
{
    visualization_msgs::msg::MarkerArray array;
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "map";
    marker.action = visualization_msgs::msg::Marker::DELETEALL;
    array.markers.push_back(marker);
    m_pub->publish(array);
}