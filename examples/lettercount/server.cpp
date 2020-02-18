#include "../../include/TaskManager.hpp"
#include "../../include/StaticController.hpp"
#include "../../include/fsutil.hpp"
#include <iostream>

using namespace std;
using namespace Backend;

int main(int argc, char const *argv[])
{   
    auto datap = make_shared<json>(fsutil::readJson("./data.json"));
    auto datap2 = make_shared<json>(fsutil::readJson("./data2.json"));
    auto datap3 = make_shared<json>(fsutil::readJson("./data3.json"));
    auto scriptp = make_shared<string>(fsutil::readScript("./a.out.js"));
    auto binp = make_shared<vector<uint8_t>>(fsutil::readBin("./a.out.wasm"));
    shared_ptr<Task> t = make_shared<Task>(scriptp, binp, *datap);
    shared_ptr<Task> t2 = make_shared<Task>(scriptp, binp, *datap2);
    shared_ptr<Task> t3 = make_shared<Task>(scriptp, binp, *datap3);
    shared_ptr<Backend::TaskQueue> tqp = make_shared<Backend::TaskQueue>();
    tqp->AddTask(t);
    tqp->AddTask(t2);
    tqp->AddTask(t3);
    cout << "TASKS ADDED" << endl;
    cout << "TASKS ADDED" << tqp->tasks[0]->getData().dump() << endl;
    cout << "TASKS ADDED" << tqp->tasks[1]->getData().dump() << endl;
    StaticController ctrl("localhost", 80, tqp);
    cout << "TASKS COMPLETED" << endl;
    vector<json> res = tqp->getResults();
    int count = 0;
    for_each(res.begin(),res.end(),
    [&](const json& j){
        count += j["instances"].get<int>();
    });
    cout << "TOTAL count of Es: " << count << endl;
    return 0;
}
