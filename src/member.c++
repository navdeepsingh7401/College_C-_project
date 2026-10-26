#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include "../include/member.h++"
#include "../include/input.h++"
#include <mysqlx/xdevapi.h>
#include <iomanip>

namespace
{
constexpr int loanPeriodDays = 30;
constexpr int finePerOverdueDay = 10;

struct LoanRecord
{
    unsigned int issueId;
    unsigned int bookId;
    unsigned int memberId;
    std::string issueDate;
    int daysHeld;
    bool returned = false;
    std::string returnDate;
};

struct LoanRecords
{
    std::vector<LoanRecord> loans;
    std::size_t unmatchedReturns = 0;
};

int calendarDayNumber(const std::string &date)
{
    int year = std::stoi(date.substr(0, 4));
    const unsigned int month = static_cast<unsigned int>(std::stoi(date.substr(5, 2)));
    const unsigned int day = static_cast<unsigned int>(std::stoi(date.substr(8, 2)));

    // Day numbers keep the fine calculation correct across daylight-saving changes.
    year -= month <= 2;
    const int era = (year >= 0 ? year : year - 399) / 400;
    const unsigned int yearOfEra = static_cast<unsigned int>(year - era * 400);
    const unsigned int adjustedMonth = month > 2 ? month - 3 : month + 9;
    const unsigned int dayOfYear = (153 * adjustedMonth + 2) / 5 + day - 1;
    const unsigned int dayOfEra =
        yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;
    return era * 146097 + static_cast<int>(dayOfEra);
}

LoanRecords loadLoanRecords(mysqlx::Schema &db)
{
    LoanRecords records;
    auto issues = db.getTable("book_issues");
    auto issueResult = issues.select(
            "issue_id", "book_id", "member_id",
            "DATE_FORMAT(issue_date, '%Y-%m-%d') AS issue_day",
            "DATEDIFF(CURRENT_DATE, issue_date) AS days_held")
        .orderBy("issue_id")
        .execute();
    auto issueRows = issueResult.fetchAll();
    std::map<std::pair<unsigned int, unsigned int>, std::vector<std::size_t>> issuesByPair;

    for (const auto &row : issueRows)
    {
        const unsigned int bookId = row[1].get<unsigned int>();
        const unsigned int memberId = row[2].get<unsigned int>();
        issuesByPair[{bookId, memberId}].push_back(records.loans.size());
        records.loans.push_back({
            row[0].get<unsigned int>(),
            bookId,
            memberId,
            row[3].get<std::string>(),
            row[4].get<int>(),
            false,
            {}
        });
    }

    // Returns are paired with the oldest checkout for that book and member.
    auto returns = db.getTable("returned_books");
    auto returnResult = returns.select(
            "book_id", "member_id",
            "DATE_FORMAT(return_date, '%Y-%m-%d') AS return_day")
        .orderBy("return_id")
        .execute();
    auto returnRows = returnResult.fetchAll();
    std::map<std::pair<unsigned int, unsigned int>, std::size_t> nextIssue;

    for (const auto &row : returnRows)
    {
        const unsigned int bookId = row[0].get<unsigned int>();
        const unsigned int memberId = row[1].get<unsigned int>();
        const auto key = std::make_pair(bookId, memberId);
        const auto matchingIssues = issuesByPair.find(key);

        if (matchingIssues == issuesByPair.end() ||
            nextIssue[key] >= matchingIssues->second.size())
        {
            ++records.unmatchedReturns;
            continue;
        }

        LoanRecord &loan = records.loans[matchingIssues->second[nextIssue[key]++]];
        loan.returned = true;
        loan.returnDate = row[2].get<std::string>();
    }

    return records;
}

int daysHeldAtReturn(const LoanRecord &loan)
{
    return calendarDayNumber(loan.returnDate) - calendarDayNumber(loan.issueDate);
}

int fineForDays(int daysHeld)
{
    return std::max(0, daysHeld - loanPeriodDays) * finePerOverdueDay;
}

void printLoanHeader()
{
    std::cout << std::left
              << std::setw(10) << "Issue ID"
              << std::setw(10) << "Book ID"
              << std::setw(12) << "Member ID"
              << std::setw(13) << "Issued"
              << std::setw(13) << "Returned"
              << std::setw(11) << "Days"
              << "Fine (Rs.)\n"
              << std::string(82, '-') << '\n';
}

void printLoan(const LoanRecord &loan, int daysHeld)
{
    std::cout << std::left
              << std::setw(10) << loan.issueId
              << std::setw(10) << loan.bookId
              << std::setw(12) << loan.memberId
              << std::setw(13) << loan.issueDate
              << std::setw(13) << (loan.returned ? loan.returnDate : "Active")
              << std::setw(11) << daysHeld
              << fineForDays(daysHeld) << '\n';
}

void warnAboutUnmatchedReturns(const LoanRecords &records)
{
    if (records.unmatchedReturns != 0)
    {
        std::cout << "Warning: " << records.unmatchedReturns
                  << " return record(s) could not be matched to a checkout.\n";
    }
}
}

// =================================
//          book Issue
// =================================

void member::bookIssue(mysqlx::Schema &db)
{
    std::cout << "==== Book Issue ====" << std::endl;

    if (!input::readInteger("Enter book ID: ", bookId) ||
        !input::readInteger("Enter member ID: ", memberId))
    {
        return;
    }

    auto books = db.getTable("books");
    auto bookResult = books.select("available_copies")
        .where("book_id = :bookId")
        .bind("bookId", bookId)
        .execute();
    auto book = bookResult.fetchOne();
    if (!book)
    {
        std::cout << "Book not found.\n";
        return;
    }
    if (book[0].get<unsigned int>() == 0)
    {
        std::cout << "No copies of that book are currently available.\n";
        return;
    }

    auto members = db.getTable("members");
    auto memberResult = members.select("member_id")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    if (!memberResult.fetchOne())
    {
        std::cout << "Member not found.\n";
        return;
    }

    auto issues = db.getTable("book_issues");
    issues.insert("book_id", "member_id")
        .values(bookId, memberId)
        .execute();
};

// =================================
//          book Return
// =================================

void member::bookReturn(mysqlx::Schema &db)
{
    std::cout << "==== Book Return ====\n";

    if (!input::readInteger("Enter book ID: ", bookId) ||
        !input::readInteger("Enter member ID: ", memberId))
    {
        return;
    }

    auto books = db.getTable("books");
    auto bookResult = books.select("book_id")
        .where("book_id = :bookId")
        .bind("bookId", bookId)
        .execute();
    if (!bookResult.fetchOne())
    {
        std::cout << "Book not found.\n";
        return;
    }

    auto members = db.getTable("members");
    auto memberResult = members.select("member_id")
        .where("member_id = :memberId")
        .bind("memberId", memberId)
        .execute();
    if (!memberResult.fetchOne())
    {
        std::cout << "Member not found.\n";
        return;
    }

    auto issues = db.getTable("book_issues");
    auto issueResult = issues.select("issue_id")
        .where("book_id = :bookId AND member_id = :memberId")
        .bind("bookId", bookId)
        .bind("memberId", memberId)
        .execute();
    auto issueRows = issueResult.fetchAll();

    auto returns = db.getTable("returned_books");
    auto returnResult = returns.select("return_id")
        .where("book_id = :bookId AND member_id = :memberId")
        .bind("bookId", bookId)
        .bind("memberId", memberId)
        .execute();
    auto returnRows = returnResult.fetchAll();

    std::size_t issueCount = 0;
    for (const auto &row : issueRows)
    {
        (void)row;
        ++issueCount;
    }
    std::size_t returnCount = 0;
    for (const auto &row : returnRows)
    {
        (void)row;
        ++returnCount;
    }

    if (returnCount >= issueCount)
    {
        std::cout << "This member has no active checkout for that book.\n";
        return;
    }

    returns.insert("book_id", "member_id")
        .values(bookId, memberId)
        .execute();
    std::cout << "Return recorded.\n";
}

// =================================
//           Active Loans
// =================================

void member::showActiveLoans(mysqlx::Schema &db)
{
    const LoanRecords records = loadLoanRecords(db);
    bool found = false;

    std::cout << "\nActive loans\n";
    printLoanHeader();
    for (const auto &loan : records.loans)
    {
        if (!loan.returned)
        {
            printLoan(loan, loan.daysHeld);
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "There are no active loans.\n";
    }
    warnAboutUnmatchedReturns(records);
}

// =================================
//          Returned Loans
// =================================

void member::showReturnedLoans(mysqlx::Schema &db)
{
    const LoanRecords records = loadLoanRecords(db);
    bool found = false;

    std::cout << "\nReturned loans (fine: Rs. " << finePerOverdueDay
              << " per day after " << loanPeriodDays << " days)\n";
    printLoanHeader();
    for (const auto &loan : records.loans)
    {
        if (loan.returned)
        {
            printLoan(loan, daysHeldAtReturn(loan));
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "There are no returned loans.\n";
    }
    warnAboutUnmatchedReturns(records);
}

// =================================
//           Overdue Loans
// =================================

void member::showOverdueLoans(mysqlx::Schema &db)
{
    const LoanRecords records = loadLoanRecords(db);
    bool found = false;

    std::cout << "\nOverdue loans (more than " << loanPeriodDays
              << " days; fine: Rs. " << finePerOverdueDay << " per day)\n";
    printLoanHeader();
    for (const auto &loan : records.loans)
    {
        const int daysHeld = loan.returned ? daysHeldAtReturn(loan) : loan.daysHeld;
        if (daysHeld > loanPeriodDays)
        {
            printLoan(loan, daysHeld);
            found = true;
        }
    }

    if (!found)
    {
        std::cout << "There are no overdue loans.\n";
    }
    warnAboutUnmatchedReturns(records);
}

// =================================
//         Total book Issue
// =================================

void member::totalBookissue(mysqlx::Schema &db)
{
    auto issues = db.getTable("book_issues");

    auto result = issues.select(
                            "issue_id",
                            "book_id",
                            "member_id",
                            "DATE_FORMAT(issue_date, '%d-%m-%Y') AS issue_date_text" // set formate of the date
                            )
                      .execute();

    auto allRows = result.fetchAll();

    std::cout << std::setw(25) << "Total Bookissue Table" << std::endl;
    // setw -> set width of text
    std::cout << std::string(53, '-') << '\n';

    std::cout << std::left
              << std::setw(12) << "Issue ID"
              << std::setw(12) << "Book ID"
              << std::setw(14) << "Member ID"
              << std::setw(15) << "Issue Date"
              << '\n';

    std::cout << std::string(53, '-') << '\n';

    for (const auto &row : allRows)
    {
        std::cout << std::left                                   // align text to left side
                  << std::setw(12) << row[0].get<unsigned int>() // .get<unsigned int>()  convert it into c++ int it is optional
                  << std::setw(12) << row[1].get<unsigned int>()
                  << std::setw(14) << row[2].get<unsigned int>()
                  << std::setw(15) << row[3].get<std::string>() // .get<std::string>() convert to stirng
                  << '\n';
    }
}
