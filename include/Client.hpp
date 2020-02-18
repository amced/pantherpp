#ifndef WASMHTTP_HPP
#define WASMHTTP_HPP

//Compile with -s FETCH=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['lengthBytesUTF8','stringToUTF8','UTF8ToString']
#include <string>
#include <emscripten/fetch.h>
#include <emscripten.h>

#include "../include/json.hpp"

using json = nlohmann::json;
using namespace std;

namespace Client {

    class PantherPP
    {

    private:
        string hostname;
        int port;

    public:
        PantherPP(const string hostname, int port);
        PantherPP(const char* hostname, int port);
        json getData();
        string sendResult(json result);
        void finalize(); //Refreshes the page;
        string getHostname();
        ~PantherPP();
    };

}


//Implementations


Client::PantherPP::PantherPP(const string hostname, int port){
    this->hostname = hostname;
    this->port = port;
};

Client::PantherPP::PantherPP(const char* hostname, int port){
    this->hostname = string(hostname);
    this->port = port;
};

json Client::PantherPP::getData(){
    string endpoint = this->getHostname() + string("/task.json");
    char* message = (char*)EM_ASM_INT({
        var request = new XMLHttpRequest();
        request.open('GET', Module.UTF8ToString($0), false);
        request.withCredentials = true;
        request.send(null);
        if(request.status == 200){
            const jsstr = request.responseText;
            const byteCount = (Module.lengthBytesUTF8(jsstr) + 1);

            const stringptr = Module._malloc(byteCount);
            Module.stringToUTF8(jsstr, stringptr, byteCount);
            return stringptr;
        } else {
            console.log("ERROR: Failed to get data from server. HTTP"+request.status);
        }
    }, endpoint.c_str());
    string s(message);
    free(message);
    return json::parse(s);
};

string Client::PantherPP::getHostname(){
    return this->hostname;
}

string Client::PantherPP::sendResult(json result){
    string endpoint = this->getHostname()+string("/result");
    char* message = (char*)EM_ASM_INT({
        var request = new XMLHttpRequest();

        request.open('POST', Module.UTF8ToString($0), false);
        request.withCredentials = true;
        request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        request.send("result="+Module.UTF8ToString($1));
        if(request.status == 200){
            const jsstr = request.responseText;
            const byteCount = (Module.lengthBytesUTF8(jsstr) + 1);

            const stringptr = Module._malloc(byteCount);
            Module.stringToUTF8(jsstr, stringptr, byteCount);
            return stringptr;
        } else {
            console.log("ERROR: Failed to get data from server. HTTP"+request.status);
        }
    }, endpoint.c_str(), result.dump().c_str());
    string s(message);
    free(message);
    return s;
};

void Client::PantherPP::finalize(){
    this->~PantherPP();
}

Client::PantherPP::~PantherPP(){
    EM_ASM(
        location.reload();
    );
}
#endif