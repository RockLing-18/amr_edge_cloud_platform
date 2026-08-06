#pragma once
#include <string>
#include <vector>

struct ZoneInfo
{
    std::string id;
    std::string name;
    std::string type;
    std::string filename;
};

struct Zone
{
    ZoneInfo info;
    std::string frame_id{"map"};
    int marker_id{-1};  // marker运行时id
    std::vector<std::pair<double,double>> polygon;
};
