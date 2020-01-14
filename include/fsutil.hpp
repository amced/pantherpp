#ifndef FSUTIL_HPP
#define FSUTIL_HPP
#include <string>
#include <vector>
#include <cinttypes>
#include "../include/json.hpp"

using json = nlohmann::json;


namespace fsutil {
    std::vector<uint8_t> readBin(std::string filename);    
    std::string readScript(std::string filename);
    json readJson(std::string filename);
}

#endif /* FSUTIL_HPP */

