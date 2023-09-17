#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"

#include <muduo/net/TcpConnection.h>
#include <unordered_map>        // 一个消息id映射一个事件处理
#include <functional>
#include <mutex>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class ChatService
{
public:
    // 单例
    static ChatService* instance();

    // 处理登录业务（id + pwd）
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务（user + password）
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 一对一聊天业务（msgid + id + from + to + msg）
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务（msgid + id + friendid）
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    // 服务器异常，业务重置方法
    void reset();
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

private:
    // 构造，注册消息以及对应的Handler回调操作
    ChatService();

    // 存储消息id和其对应的业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;
};

#endif