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
    std::vector<std::pair<double,double>> polygon;
};
