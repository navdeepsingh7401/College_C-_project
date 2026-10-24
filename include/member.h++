#pragma once
#include <string>

class member{
    public:
    void bookIssue();
    void bookReturn();


    protected:
    // bookIssue 
    int bookId;   // used for find book 
    int day,month,year;  // day of issue book and submit 
    int memberId;     // also use this for bookSubmit 

    // bookSubmit
    int bookSubmitId;


};