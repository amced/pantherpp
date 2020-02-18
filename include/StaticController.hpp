#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP

#include "../include/httplib.h"
#include "../include/TaskManager.hpp"

#include <string>
#include <memory>
#include <sstream>
#include <map>

#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp> 
#include <boost/uuid/uuid_io.hpp>         


using namespace Backend;
using namespace std;
using namespace httplib;

class StaticController
{
    public:
        StaticController(const char* hostname, int port, shared_ptr<Backend::TaskQueue> tqp);

    private:
    

        bool completionChecks(const httplib::Request& req,httplib::Response& res);

        string getTaskId(const string& sessionId);
        string getTaskId(const httplib::Request& req);
        string checkIn(httplib::Response& res, const string task_id);

        bool seenBefore(const httplib::Request& req);
        string getSessionId(const httplib::Request& req);



        int getPort();
        string getHostname();

        map<string,string> sessionToTaskMap; //key is sessionid, value is task_id
        shared_ptr<Backend::TaskQueue> tqp;
        string cookieKey = string("pantherSessionId");
        httplib::Server svr;
        int port;
        string hostname;
        boost::uuids::random_generator generator;
};

#endif /* HTTPCONTROLLER_HPP */