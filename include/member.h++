#pragma once
#include <mysqlx/xdevapi.h>
class member{
    public:
    void bookIssue(mysqlx::Schema& db);
    void bookReturn(mysqlx::Schema& db);

    
    void totalBookReturn(mysqlx::Schema& db);
    void totalBookissue(mysqlx::Schema& db);


    protected:
    // bookIssue 
    int bookId;   // used for find book 
    int day,month,year;  // day of issue book and submit 
    int memberId;     // also use this for bookSubmit 

    // bookSubmit
    int bookSubmitId;


};