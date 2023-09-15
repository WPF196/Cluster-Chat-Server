#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

// 聊天服务器主类
class ChatServer
{
public:
    // 构造：初始化服务器对象
    ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg);
    
    // 启动服务器
    void start();

private:
    // 上报连接相关信息的回调函数
    void onConnection(const TcpConnectionPtr&);
    // 上报读写事件相关信息的回调函数
    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

    TcpServer _server;      // muduo库中的服务器类对象
    EventLoop *_loop;       // muduo库中指向事件循环对象的指针
};


#endif