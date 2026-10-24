#pragma once
#include <string>


class library{
    // data member 

    private:
    // book details
    std::string bookName;
    std::string autherName;
    int bookID , bookStock;

    // member details

    std::string memberName;
    std::string memberID;
    std::string memberContact;


    public:
    // member function 
    void addBook();
    void addMember();
};