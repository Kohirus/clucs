#include "groupmodel.hpp"
#include "db.hpp"
#include <mysql/mysql.h>

bool GroupModel::createGroup(Group& group) {
    char sql[1024] = { 0 };
    sprintf(sql, "INSERT INTO AllGroup(groupname, groupdesc) VALUES('%s', '%s')",
        group.getName().c_str(), group.getDesc().c_str());

    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }

    return false;
}

void GroupModel::addGroup(int userid, int groupid, const string& role) {
    char sql[1024] = { 0 };
    sprintf(sql, "INSERT INTO GroupUser VALUES(%d, %d, '%s')",
        groupid, userid, role.c_str());

    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}

vector<Group> GroupModel::queryGroups(int userid) {
    /*
        先根据userid在groupuser表中查询出该用户所属的群组信息
        再根据群组信息，查询属于该群组的所有用户的userid，并且和user表进行多表联合查询，
        查出用户的详细信息
    */
    char sql[1024] = { 0 };
    sprintf(sql, "SELECT a.id, a.groupname, a.groupdesc FROM AllGroup a INNER JOIN \
         GroupUser b ON a.id = b.groupid WHERE b.userid=%d",
        userid);

    vector<Group> groupVec;

    MySQL mysql;
    if (mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            MYSQL_ROW row;
            // 查出userid所有的群组信息
            while ((row = mysql_fetch_row(res)) != nullptr) {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);
            }
            mysql_free_result(res);
        }
    }

    // 查询群组的用户信息
    for (Group& group : groupVec) {
        sprintf(sql, "SELECT a.id, a.name, a.state, b.grouprole FROM User a \
            INNER JOIN GroupUser b ON b.userid = a.id WHERE b.groupid=%d",
            group.getId());

        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}

vector<int> GroupModel::queryGroupUsers(int userid, int groupid) {
    char sql[1024] = { 0 };
    sprintf(sql, "SELECT userid FROM GroupUser WHERE groupid = %d AND userid != %d", groupid, userid);

    vector<int> idVec;
    MySQL       mysql;
    if (mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr) {
                idVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }
    return idVec;
}
