/*
@ json 测试代码，与项目无关
*/

#include "../thirdparty/json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

// json 序列化示例1，常规序列化
string func1(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhangsan";
    js["to"] = "li si";
    js["msg"] = "hello, 你好";

    // 通过 dump 方法将 json 转为字符串
    string sendBuf = js.dump();
    cout << sendBuf.c_str() << endl << endl;

    return sendBuf;
}

// json 序列化示例2，复杂序列化
string func2(){
    json js;

    js["id"] = {1, 2, 3, 4, 5};
    js["name"] = "zhangsan";
    
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china";
    js["msg"] = {{"zhang san", "hello world"}, {"liu shuo", "hello china"}};
    
    string sendBuf = js.dump();
    cout << sendBuf.c_str() << endl << endl;

    return sendBuf;
}

// json 序列化示例3，容器序列化
string func3(){
    json js;
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "华山"});
    m.insert({3, "泰山"});
    js["path"] = m;

    string sendBuf = js.dump();
    cout << sendBuf.c_str() << endl << endl;

    return sendBuf;
}

int main(){
    string recvBuf = func1();
    string recvBuf2 = func2();
    string recvBuf3 = func3();

    // 反序列化
    json buf = json::parse(recvBuf);
    cout << buf["msg_type"] << endl;
    cout << buf["from"] << endl;
    cout << buf["to"] << endl;
    cout << buf["msg"] << endl;

    return 0;
}