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

//void ThreadUser() { //�߳����
//    cout << "���߳̿�ʼ" << "\n";
//    for (int i = 0; i < 100; ++i) { //��ռѭ��
//        cout << "���̵߳�" << i << "��ѭ����ռ��" << "\n"; //�����Ϣ
//        Sleep(100); //��ռ��ʱ
//    }
//    cout << "���߳̽���" << "\n";
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
    //��Ҫ�޸ĵ����ݿ���Ϣ,��¼��,����,����
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "qgydb";
    int n = 0;
    //�����н���

    std::thread t_server(Http_conn::server_main); // pass
    std::thread t_client(Http_conn::client_main); // pass 
    t_server.join();
    t_client.join();
    std::cout << "Final value of n is " << n << '\n';
    cout << "���߳̽���" << "\n";
    system("pause");
    return 0;
}