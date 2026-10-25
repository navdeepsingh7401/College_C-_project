#pragma once
#include <string>
#include <vector>
#include <mysqlx/xdevapi.h>


class library{
    // data member 

    protected:
    // book details
    std::string bookName;
    std::string autherName;
    int bookID , bookStock, option;

    // member details

    std::string memberName;
    int memberId;
    std::string memberContact;


    public:
    // member function 
    void addBook(mysqlx::Schema& db);
    void addMember(mysqlx::Schema& db);
    void showAllMembers(mysqlx::Schema& db);
    void searchMembers(mysqlx::Schema& db);
    void updateMembersInfo(mysqlx::Schema& db);
};