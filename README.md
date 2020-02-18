# pantherpp
###### Distributed computing using Web Browsers.
Reimplementation of the [Panther Framework](https://static.epcc.ed.ac.uk/dissertations/hpc-msc/2017-2018/Simon_Newby-s1687239_-_Dissertation_-_PDF1.pdf) (PantherV1) in C++. Allows for distributed computation using volunteering Web Browsers, using a master/worker pattern.



### Expanded features from PantherV1:
+ Allows for different tasks on different data, thanks to a redesigned task manager.
+ Modular Backend classes, including Task and TaskQueue. Designed to serve WebAssembly-capable Tasks.
+ **WIP** Data transfers via HTTP or WebSocket.
+ **WIP** C++ only client library.


##Libraries
+ [JSON for Modern C++](https://github.com/nlohmann/json)
+ [A C++ header-only HTTP/HTTPS server and client library](https://github.com/yhirose/cpp-httplib)
+ [Boost-Random](https://www.boost.org/)