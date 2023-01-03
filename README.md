# cppSocketDemo
cpp下通过socket实现C/S的TCP/UDP通信样例

介绍：[C++ 使用Socket实现主机间的UDP/TCP通信](https://hctra.cn/index.php/archives/918/)

server端代码做了跨平台（POSIX/Windows），client端代码为Windows兼容

c++使用c++11以上标准编译

client记得链接库-lwsock32

server的多客户版本如果在POSIX平台编译记得链接库 -lpthread
