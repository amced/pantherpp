#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP
#include "../include/json.hpp"

#include <functional>
#include <string>
#include <vector>
#include <map>
#include <cinttypes>

using namespace std;
using json = nlohmann::json;

namespace Backend {
class Task 
{
    public:
        vector<uint8_t> getBinary();
        string getScript();
        shared_ptr<vector<uint8_t>> getBinaryPtr();
        shared_ptr<string> getScriptPtr();
        json getData();
        json getResult();
        bool IsComplete();
        bool IsDispatched();
        void MarkDispatched(string task_id);
        void FillResult(const json result);
        void MarkCompleted();
        string getTaskId();
        Task(shared_ptr<string>& script, shared_ptr<vector<uint8_t>>& binary, json data);

    private:
        shared_ptr<vector<uint8_t>> binary; 
        shared_ptr<string> script;
        json data;
        json result;
        string task_id = string("");
        bool completed = false;
        bool dispatched = false;

};

class TaskQueue
{
    public:
        TaskQueue();
        void AddTask(const shared_ptr<Task>& task);
        void ManyAddTask(const vector<shared_ptr<Task>>& tasks_in);
        shared_ptr<Task> getNextTask();
        shared_ptr<Task> DispatchNextTask();
        void MarkCompleted(string task_id, json& result);
        int nTasks();
        int nTasksRemaining();
        int nTasksOutstanding();
        vector<shared_ptr<Task>> tasks;


    private:
        int remaining;

};

};

#endif