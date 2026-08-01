# amr_warehouse_collaborative_system
基于ROS2的智能仓储多机器人协同系统,一个完整的云边协同仓储机器人仿真平台。机器人端基于 ROS2 Humble + Nav2 实现自主导航与避障；视觉端通过 RGB-D 相机完成货箱颜色识别与 3D 空间定位；执行端驱动 4-DOF 机械臂配合力反馈传感器完成抓取确认。云端通过 FastAPI + MQTT 实现任务调度与多机协同，支持动态任务分配、优先级出库逻辑和异常上报。Gazebo 仿真验证，Python/C++ 混合开发。
