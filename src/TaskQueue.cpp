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
#include <iostream>

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using namespace std;
using namespace Backend;
using json = nlohmann::json;

Backend::TaskQueue::TaskQueue()
{
    this->remaining = 0;
}

void Backend::TaskQueue::AddTask(const shared_ptr<Task>& task)
{
    this->tasks.push_back(task);
}

void Backend::TaskQueue::ManyAddTask(const vector<shared_ptr<Task>>& tasks_in)
{
    copy(tasks_in.begin(), tasks_in.end(), inserter(this->tasks, this->tasks.end()));
}

int Backend::TaskQueue::nTasksRemaining()
{
    return count_if(
        this->tasks.begin(),
        this->tasks.end(),
        [&](shared_ptr<Task> tp){
            return !(tp->IsDispatched());
        }
    );
}

int Backend::TaskQueue::nTasksOutstanding()
{
    return count_if(
        this->tasks.begin(),
        this->tasks.end(),
        [&](shared_ptr<Task> tp){
            return !(tp->IsComplete());
        }
    );
}


shared_ptr<Task> Backend::TaskQueue::getNextTask()
{
       
    vector<shared_ptr<Task>>::iterator p = find_if(
        tasks.begin(),
        tasks.end(),
        [](shared_ptr<Task> const& t){
            return !(*t).IsDispatched();
        }
    );
    if(p == tasks.end()) throw range_error("Backend::TaskQueue::getNextTask(), std::find_if() : No element found.");
    return *p;

}

shared_ptr<Task> Backend::TaskQueue::DispatchNextTask()
{
    shared_ptr<Task> p = this->getNextTask();
    string chars(
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "1234567890");

    boost::random::random_device rng;

    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    std::string s;

    for(int i = 0; i < 24; ++i) {
        s += chars[index_dist(rng)];
    }

    p->MarkDispatched(s);
    return p;
}

void Backend::TaskQueue::MarkCompleted(string task_id, json& result)
{
    shared_ptr<Task> tp = *find_if(
        this->tasks.begin(),
        this->tasks.end(),
        [=](shared_ptr<Task> t){
            return t->getTaskId() == task_id;
        }
    );
    tp->FillResult(result);
    tp->MarkCompleted();
}


int Backend::TaskQueue::nTasks()
{
    return this->tasks.size();
}