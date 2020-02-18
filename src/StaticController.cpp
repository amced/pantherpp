#include "../include/StaticController.hpp"
#include "../include/TaskManager.hpp"
#include "../include/httplib.h"

#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <regex>
#include <vector>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>  
//WIP!

using namespace std;
using namespace httplib;
using namespace Backend;
using json = nlohmann::json;

bool StaticController::completionChecks(const httplib::Request& req,httplib::Response& res){
    if(tqp->nTasksRemaining() == 0){
        res.status = 404;
        return true;
    } else {
        return false;
    }
};

string StaticController::getTaskId(const string& sessionId){
    auto i = this->sessionToTaskMap.find(sessionId);
    if (i == this->sessionToTaskMap.end()) throw range_error("StaticController::getTaskId(const string& sessionId): Key not in map.");
    return i->second;
};
string StaticController::getTaskId(const httplib::Request& req){
    if(req.has_header("Cookie")){ // TODO find better alternative for parsing cookies
        string cookie = req.get_header_value("Cookie");
        regex key("("+this->cookieKey+"=)+");
        string repl("");
        auto pantherSessionId = regex_replace(cookie,key,repl);
        return this->getTaskId(pantherSessionId);
    }
    else
    {
        throw logic_error("StaticController::getTaskId(const httplib::Request& req): No Cookie present;");
    }
};


string StaticController::checkIn(httplib::Response& res,const string task_id)
{ //only call if seenBefore() returns false
    boost::uuids::uuid uuid = this->generator();
    stringstream ss;
    ss << uuid;
    string sessionId = ss.str();

    this->sessionToTaskMap[sessionId] = task_id;
    res.set_header("Set-Cookie",this->cookieKey+string("=")+sessionId);
    return sessionId;
};


bool StaticController::seenBefore(const httplib::Request& req)
{
    if(req.has_header("Cookie")){ // TODO find better alternative for parsing cookies
        string cookie = req.get_header_value("Cookie");
        regex key("("+this->cookieKey+"=)+");
        string repl("");
        auto pantherSessionId = regex_replace(cookie,key,repl);
        return !(this->sessionToTaskMap.end() == this->sessionToTaskMap.find(pantherSessionId));
    }
    else
    {
        return false;
    }
};




string StaticController::getSessionId(const httplib::Request& req){
    string cookie = req.get_header_value("Cookie");
    regex key("("+this->cookieKey+"=)+");
    string repl("");
    auto pantherSessionId = regex_replace(cookie,key,repl);
    return pantherSessionId;
};







StaticController::StaticController(const char* hostname, int port, shared_ptr<Backend::TaskQueue> tqp){
    this->hostname = std::string(hostname);
    this->port = port;
    this->tqp = tqp;

    this->svr.Get("/task.js", [&](const Request& req, Response& res) {
        if (this->completionChecks(req, res)){      
                res.set_content("console.log('No more tasks')", "application/javascript");
        }
        else
        {
            res.set_header("Access-Control-Allow-Origin", req.get_header_value("*"));
            if (this->seenBefore(req))
            {   
                const string sessionId = this->getSessionId(req);
                auto taskptr = tqp->DispatchNextTask();
                this->sessionToTaskMap[sessionId] = taskptr->getTaskId();
                res.set_content(taskptr->getScript(), "application/javascript");
            }
            else 
            {
                auto taskptr = tqp->DispatchNextTask();
                auto task_id = taskptr->getTaskId();
                auto sessionId = this->checkIn(res,task_id);
                res.set_content(taskptr->getScript(), "application/javascript");
            }
        }
    });

    this->svr.Get("/a.out.wasm", [&](const Request& req, Response& res) {
        if(this->seenBefore(req)){
            res.set_header("Access-Control-Allow-Origin", req.get_header_value("Origin"));
            res.set_header("Access-Control-Allow-Credentials", "true");
            const string sessionId = this->getSessionId(req);
            const string task_id = this->getTaskId(sessionId);
            auto taskptr = tqp->getTaskById(task_id);
            const vector<uint8_t> bin = taskptr->getBinary();
            res.set_content((char *)bin.data(), bin.size(), "application/wasm");
        }
        else
        {
            res.status = 404;
        }
    });

    //Data Route
    this->svr.Get("/task.json", [&](const Request& req, Response& res) {
        if(this->seenBefore(req)){
            res.set_header("Access-Control-Allow-Origin", req.get_header_value("Origin"));
            res.set_header("Access-Control-Allow-Credentials", "true");
            const string sessionId = this->getSessionId(req);
            const string task_id = this->getTaskId(sessionId);
            const json taskdata = tqp->getTaskById(task_id)->getData();
            auto taskptr = tqp->getTaskById(task_id);
            res.set_content(taskdata.dump(), "application/json");
        }
        else
        {
            res.status = 404;
        }
    });

    //Return route
    this->svr.Post("/result",[&](const Request& req, Response& res) {
        if(this->seenBefore(req) && req.has_param("result")){
            res.set_header("Access-Control-Allow-Origin", req.get_header_value("Origin"));
            res.set_header("Access-Control-Allow-Credentials", "true");

            string ress = req.get_param_value("result");
            json result = json::parse(ress);
            tqp->MarkCompleted(this->getTaskId(req), result);
            res.set_content(string("success"), "application/json");
            //Verbose
            cout << "Tasks remaining: " << this->tqp->nTasksRemaining() << endl;
            cout << "Task result: " << this->tqp->getTaskById(result["task_id"].get<string>())->getResult() << endl;
            if(this->completionChecks(req, res)) this->svr.stop();
        } else {
            res.status = 404;
        }
    });

    this->svr.listen(hostname,port);
}






std::string StaticController::getHostname(){
    return this->hostname;
};

int StaticController::getPort(){
    return this->port;
};