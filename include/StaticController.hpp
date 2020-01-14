#ifndef CONTROLLERS_HPP
#define CONTROLLERS_HPP

#include "../include/TaskManager.hpp"
#include <string>
#include <memory>


using namespace Backend;
using namespace std;

class StaticController
{
    public:
        StaticController(const char* hostname, int port, shared_ptr<Backend::TaskQueue>& tqp);

    private:
        shared_ptr<Task> ptr;
        int getPort();
        string getHostname();
        int port;
        string hostname;
};

#endif /* HTTPCONTROLLER_HPP */