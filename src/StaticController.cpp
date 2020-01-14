#include "../include/StaticController.hpp"
#include "../include/TaskManager.hpp"
#include "../include/httplib.h"


//WIP!

using namespace std;
using namespace httplib;
using namespace Backend;

int StaticController::getPort(){
    return this->port;
};



std::string StaticController::getHostname(){
    return this->hostname;
};



StaticController::StaticController(const char* hostname, int port, shared_ptr<Backend::TaskQueue>& tqp){
    this->hostname = std::string(hostname);
    this->port = port;
    Server svr;
    //Static routes
    svr.Get("/task.wasm", [&](const Request& req, Response& res) {
        vector<uint8_t> bin = tqp->getNextTask()->getBinary() ;
        res.set_content((char *)bin.data(), bin.size(), "application/wasm");
    });

    svr.Get("/task.js", [&](const Request& req, Response& res) {
        shared_ptr<string> strp = tqp->getNextTask()->getScriptPtr();
        res.set_content(*strp, "application/javascript");
    });

    //Data Route
    svr.Get("/task.json", [&](const Request& req, Response& res) {
        json data = tqp->DispatchNextTask()->getData();
        res.set_content(data.dump(), "application/json");
    });

    //Return route
    svr.Post("/result",[&](const Request& req, Response& res) {
        if(req.has_param("result")){
            json result = json::parse(req.get_param_value("result"));
            tqp->MarkCompleted(result["task_id"].get<string>(), result);
            string rstr("success");
            res.set_content(rstr.c_str(), "application/json");
        } else {
            res.status = 404;
        }
    });

    svr.listen(hostname,port);
}

