#pragma once
#include <string>
#include <vector>


class library{
    // data member 

    protected:
    // book details
    std::string bookName;
    std::string autherName;
    int bookID , bookStock;

    // member details

    std::string memberName;
    int memberId;
    std::string memberContact;


    public:
    // member function 
    void addBook();
    void addMember();
};