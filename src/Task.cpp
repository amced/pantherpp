#include "../include/TaskManager.hpp"
#include "../include/json.hpp"

#include <functional>
#include <string>
#include <vector>
#include <cinttypes>
#include <stdexcept>
#include <random>
#include <chrono>
#include <algorithm>
#include <memory>

using namespace std;
using namespace Backend;
using json = nlohmann::json;

Backend::Task::Task(shared_ptr<string>& script, shared_ptr<vector<uint8_t>>& binary, json data)
{
    this->script = script;
    this->binary = binary;
    this->data = data;
}

vector<uint8_t> Backend::Task::getBinary()
{
    return (*this->binary);
}

string Backend::Task::getScript()
{
    return (*this->script);
}

shared_ptr<vector<uint8_t>> Backend::Task::getBinaryPtr()
{
    return this->binary;
}

shared_ptr<string> Backend::Task::getScriptPtr()
{
    return this->script;
}

json Backend::Task::getData()
{
    return this->data;
}

json Backend::Task::getResult()
{
    if(!this->completed)
    {
        throw logic_error(string("Task not yet complete"));
    }
    return this->result;
}

bool Backend::Task::IsComplete()
{
    return this->completed;
}


bool Backend::Task::IsDispatched()
{
    return this->dispatched;
}

void Backend::Task::FillResult(const json result)
{
    this->result = result;
}

void Backend::Task::MarkCompleted(){
    this->completed = true;
}

void Backend::Task::MarkDispatched(string task_id)
{
    this->dispatched = true;
    this->data["task_id"] = task_id;
    this->task_id = task_id;
};

string Backend::Task::getTaskId()
{
    return this->task_id;
};
