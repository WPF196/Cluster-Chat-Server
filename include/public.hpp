#ifndef PUBLIC_H
#define PUBLIC_H

/**
 * server 和 client的公共文件
*/

enum EnMsgType
{
    LOGIN_MSG = 1,  // 登录
    LOGIN_MSG_ACK,  // 登录响应消息
    REG_MSG,        // 注册
    REG_MSG_ACK     // 注册响应消息
};

#endif