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
    _msgHandlerMap.insert({ONE_CHAT_MSG, bind(&ChatService::oneChat, this, _1, _2, _3)});
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
    int id = js["id"].get<int>();
    string pwd = js["password"];

    User user = _userModel.query(id);

    if(user.getId() == id && user.getPwd() == pwd){
        if(user.getState() == "online"){
            // 该用户已经登录，不允许重复登录
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "该账户已经登录，无法重复登录！";
            conn->send(response.dump());
        }
        else{
            // 登录成功
            {
                // 智能锁，防止死锁，遇到下一个 } 自动析构
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({id, conn});
            }

            user.setState("online");
            // 数据库的并发由mysql_server来保证安全
            _userModel.updateState(user);

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            conn->send(response.dump());
        }
        
    }
    else{
        // 用户不存在 或 用户存在但密码错误
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "用户名或者密码错误";
        conn->send(response.dump());
    }
}

void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);

    bool state = _userModel.insert(user);
    if(state){
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else{
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{   
    User user;
    {
        lock_guard<mutex> lock(_connMutex);
        for(auto it  = _userConnMap.begin(); it != _userConnMap.end(); ++it)
            if(it->second == conn){
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
    }
    if(user.getId() != -1){
        user.setState("offline");
        _userModel.updateState(user);
    }
}

void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int toid = js["to"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if(it != _userConnMap.end()){
            // toid在线，服务器主动推送消息给toid用户
            it->second->send(js.dump());
            return;
        }
    }
}