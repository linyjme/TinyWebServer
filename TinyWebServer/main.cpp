#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<ctime>
#include<conio.h>
#include<Windows.h>
#include "config.h"

using namespace std;

int main(int argc, char* argv[])
{
    //需要修改的数据库信息,登录名,密码,库名
    std::string user = "root";
    std::string passwd = "root";
    std::string databasename = "qgydb";

    //命令行解析
    Config config;
}