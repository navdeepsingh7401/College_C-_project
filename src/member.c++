#include <iostream>
#include <string>
#include "../include/member.h++"


void member::bookIssue(){
    std::cout << "==== Book Issue ====" << std::endl;

    std::cout << "Enter Issue book : ";
    std::cin >> bookId;
    

    std::cout << "Enter Date of Issue in dd mm yy : ";
    std::cin >> day >> month >> year;

    
    std::cout << "Issue Member ID : ";
    std::cin >> memberId;
    
    std::cout << std::endl;

    // Showing input  detail 
    std::cout << "book Issue Member Id  : " << memberId << std::endl;
    std::cout << "book Issue On         : " << day << "/" << month << "/" << year << std::endl;
    std::cout << "book Issue Id         : " << bookId << std::endl;

};

void member::bookReturn(){
    std::cout << "==== Book Submition ====" << std::endl;
    std::cout << "Submission  book Id : ";
    std::cin >> bookId;
    
    std::cout << "Enter Date of Submission in dd mm yy : ";
    std::cin >> day >> month >> year;

    
    std::cout << "Submission Member ID : ";
    std::cin >> memberId;

    std::cout << std::endl;

    // Showing input detail 
    std::cout << "book Issue Member Id  : " << memberId << std::endl;
    std::cout << "book Return On         : " << day << "/" << month << "/" << year << std::endl;
    std::cout << "Submission Issue Id     : " << bookId << std::endl;

};
