#ifndef GROUPMODEL_H
#define GROUPMPDEL_H

#include "group.hpp"

#include <string>
#include <vector>

using namespace std;

// 维护群组信息的操作接口
class GroupModel
{
public:
    // 创建群组
    bool createGroup(Group &group);
    // 加入群组
    void addGroup(int userid, int groupid, string role);
    // 查询用户所在群组的信息
    vector<Group> queryGroups(int userid);
    // 根据groupid查询用户id列表（除userid自己，将信息发给其他组员）
    vector<int> queryGroupUsers(int userid, int groupid);
};

#endif