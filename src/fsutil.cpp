#include "../include/fsutil.hpp"


#include <string>
#include <vector>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <iterator>
#include <exception>
#include <sstream>
#include <regex>
#include "../include/json.hpp"

std::vector<std::uint8_t> fsutil::readBin(const std::string filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);
    if(!file) throw std::invalid_argument(std::string("Could not open binary file ") + filename);
    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<std::uint8_t> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());
    return vec;

}

std::string fsutil::readScript(const std::string filename)
{   //REPLACES FETCH PARAMETERS TO ALLOW COOKIES
        std::ifstream file(filename);
        if(!file) throw std::invalid_argument(std::string("Could not open script file ") + filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::regex key("same-origin");
        std::string repl("include");
        return std::regex_replace(buffer.str(), key, repl);
}

nlohmann::json fsutil::readJson(const std::string filename)
{
    nlohmann::json buf;
    std::ifstream file(filename);
    if(!file) throw std::invalid_argument(std::string("Could not open JSON file ") + filename);
    file >> buf;
    return buf;
}