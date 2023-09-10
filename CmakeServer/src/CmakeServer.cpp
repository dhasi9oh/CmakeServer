// CmakeServer.cpp: 定义应用程序的入口点。
//
#include <iostream>
#include <string>
#include "MysqlConn.h"

int main()
{
    MysqlConnPool::instance()->init(1, "localhost", "root", "qq1977473447", "studentdatas");
    MysqlConnRAII con;
    con.query("select * from usr");
    MysqlResult res = con.result();
    for (auto row : res) {
        for (auto word : row)
            std::cout << word.second << " ";
        std::cout << std::endl;
    }
	return 0;
}
