#pragma once
#include <string>
#include <vector>
#include "data_define.h"

class YamlZoneManager
{
public:
    bool load(const std::string& filename);
    bool addZone(const Zone& zone);
    bool updateZone(const Zone& zone);
    bool removeZone(const std::string& id);
    bool save();

private:
    std::string m_filename;
    std::vector<Zone> m_vZone;
};