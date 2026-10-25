#include <iostream>
#include <fstream>
#include <limits>           // set litmit of code
#include "library.h++"      // custom header file 1
#include "member.h++"       //custom header file 2
#include <mysqlx/xdevapi.h> // help to connect with data base provide api
using namespace std;

int main()
{
    library l;
    member m;

    // connecting database

    try
    {
        mysqlx::Session session(
            "mysqlx://root:sumqes-jyGgi0-wodsuv@127.0.0.1:33060");
        // connect variable with schema
        auto db = session.getSchema("library_management");

        //  operation menu
        int option;

        cout << "==== Operation ====" << endl;

        cout << "1 Add Member " << endl;
        cout << "2 Add books " << endl;
        cout << "3 Book Issue" << endl;
        cout << "4 Book Submition" << endl;
        cout << "5 Total Bookissue" << endl;
        cout << "select option : ";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove the left over white space of getline

        switch (option)
        {
        case 1:
            l.addMember(db);
            break;

        case 2:
            l.addBook(db);
            break;

        case 3:
            m.bookIssue(db);
            break;

        case 4:
            m.bookReturn(db);
            break;
        case 5:
            m.totalBookissue(db);
            break;

        default:
            cout << "please Select correct option " << endl;
            break;
        }
    }
    catch (const mysqlx::Error &e)
    {
        std::cerr << "MySQL error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}