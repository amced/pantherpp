#include "../../include/Client.hpp"
#include "../../include/json.hpp"
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;
using json = nlohmann::json;


int main(int argc, char const *argv[])
{
    Client::PantherPP cl("http://localhost", 80);
    json data = cl.getData();

    const char k{'e'};
    int count = 0;
    string s=data["text"].get<string>();
    data.erase("text");

    for_each(s.begin(),s.end(),
    [&](const char c){
        if(c == k) count++;
    });
    data["instances"] = count;

    cout << "count of Es: " << count << endl;

    cl.sendResult(data);

    return 0;
}
