#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<ctime>
#include<conio.h>
#include<Windows.h>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include "config.h"
#include "http_conn.h"
using namespace std;

//void ThreadUser() { //线程入口
//    cout << "子线程开始" << "\n";
//    for (int i = 0; i < 100; ++i) { //抢占循环
//        cout << "子线程第" << i << "次循环抢占；" << "\n"; //输出信息
//        Sleep(100); //抢占延时
//    }
//    cout << "子线程结束" << "\n";
//}
//
//void f1(int n)
//{
//    for (int i = 0; i < 5; ++i) {
//        std::cout << "Thread " << n << " executing\n";
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    }
//}
//
//void f2(int& n)
//{
//    for (int i = 0; i < 5; ++i) {
//        std::cout << "Thread 2 executing\n";
//        ++n;
//        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    }
//}

int main(int argc, char* argv[])
{
    //需要修改的数据库信息,登录名,密码,库名
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "qgydb";
    int n = 0;
    //命令行解析

    std::thread t_server(Http_conn::server_main); // pass
    std::thread t_client(Http_conn::client_main); // pass 
    t_server.join();
    t_client.join();
    std::cout << "Final value of n is " << n << '\n';
    cout << "主线程结束" << "\n";
    system("pause");
    return 0;
}