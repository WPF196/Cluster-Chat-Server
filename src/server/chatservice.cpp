#include "chatservice.hpp"
#include "public.hpp"

#include <muduo/base/Logging.h>

using namespace muduo;

ChatService* ChatService::instance()
{
    static ChatService service;
    return &service;
}

ChatService::ChatService()
{
    _msgHandlerMap.insert({LOGIN_MSG, bind(&ChatService::login, this, _1, _2, _3)});
    _msgHandlerMap.insert({REG_MSG, bind(&ChatService::reg, this, _1, _2, _3)});
}

MsgHandler ChatService::getHandler(int msgid)
{   
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = _msgHandlerMap.find(msgid);
    if(it == _msgHandlerMap.end()){
        // 返回一个空操作
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp){
            LOG_ERROR << "msgid:" << msgid << " can not find handler";
        };
    }
    else
        return _msgHandlerMap[msgid];
}

void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    LOG_INFO << "do login service!!!";
}

void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    LOG_INFO << "do reg service!!!";
}