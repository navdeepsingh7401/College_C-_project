#include "report.h++"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "input.h++"

namespace
{
struct Report
{
    std::string name;
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
};

std::string csvValue(const std::string &value)
{
    std::string escaped = "\"";
    for (char character : value)
    {
        if (character == '"')
        {
            escaped += '"';
        }
        escaped += character;
    }
    escaped += '"';
    return escaped;
}

std::string jsonValue(const std::string &value)
{
    std::string escaped = "\"";
    for (char character : value)
    {
        switch (character)
        {
        case '"':
            escaped += "\\\"";
            break;
        case '\\':
            escaped += "\\\\";
            break;
        case '\n':
            escaped += "\\n";
            break;
        case '\r':
            escaped += "\\r";
            break;
        case '\t':
            escaped += "\\t";
            break;
        default:
            escaped += character;
            break;
        }
    }
    escaped += '"';
    return escaped;
}

void writeRow(std::ofstream &file, const std::vector<std::string> &row, int format)
{
    for (std::size_t index = 0; index < row.size(); ++index)
    {
        if (index != 0)
        {
            file << (format == 2 ? '\t' : ',');
        }
        file << (format == 3 ? jsonValue(row[index]) : row[index]);
    }
    file << '\n';
}

Report loadReport(mysqlx::Schema &db, int choice)
{
    Report report;
    if (choice == 1)
    {
        report.name = "books";
        report.headers = {"Book ID", "Title", "Author", "Total Copies", "Available Copies"};
        auto result = db.getTable("books")
            .select("book_id", "title", "author", "total_copies", "available_copies")
            .orderBy("title")
            .execute();
        for (const auto &row : result.fetchAll())
        {
            report.rows.push_back({
                std::to_string(row[0].get<unsigned int>()),
                row[1].get<std::string>(),
                row[2].get<std::string>(),
                std::to_string(row[3].get<unsigned int>()),
                std::to_string(row[4].get<unsigned int>())
            });
        }
    }
    else if (choice == 2)
    {
        report.name = "members";
        report.headers = {"Member ID", "Name", "Contact"};
        auto result = db.getTable("members")
            .select("member_id", "name", "contact")
            .orderBy("name")
            .execute();
        for (const auto &row : result.fetchAll())
        {
            report.rows.push_back({
                std::to_string(row[0].get<unsigned int>()),
                row[1].get<std::string>(),
                row[2].get<std::string>()
            });
        }
    }
    else if (choice == 3)
    {
        report.name = "checkouts";
        report.headers = {"Issue ID", "Book ID", "Member ID", "Issue Date"};
        auto result = db.getTable("book_issues")
            .select("issue_id", "book_id", "member_id",
                    "DATE_FORMAT(issue_date, '%Y-%m-%d %H:%i:%s')")
            .orderBy("issue_id")
            .execute();
        for (const auto &row : result.fetchAll())
        {
            report.rows.push_back({
                std::to_string(row[0].get<unsigned int>()),
                std::to_string(row[1].get<unsigned int>()),
                std::to_string(row[2].get<unsigned int>()),
                row[3].get<std::string>()
            });
        }
    }
    else
    {
        report.name = "returns";
        report.headers = {"Return ID", "Book ID", "Member ID", "Return Date"};
        auto result = db.getTable("returned_books")
            .select("return_id", "book_id", "member_id",
                    "DATE_FORMAT(return_date, '%Y-%m-%d %H:%i:%s')")
            .orderBy("return_id")
            .execute();
        for (const auto &row : result.fetchAll())
        {
            report.rows.push_back({
                std::to_string(row[0].get<unsigned int>()),
                std::to_string(row[1].get<unsigned int>()),
                std::to_string(row[2].get<unsigned int>()),
                row[3].get<std::string>()
            });
        }
    }
    return report;
}
}

void exportReport(mysqlx::Schema &db)
{
    std::cout << "\nExport data\n"
              << "1. Books\n"
              << "2. Members\n"
              << "3. Checkout records\n"
              << "4. Return records\n";
    int reportChoice;
    if (!input::readInteger("Choose report: ", reportChoice) || reportChoice > 4)
    {
        std::cout << "Please select a report from 1 to 4.\n";
        return;
    }

    std::cout << "\nChoose format\n"
              << "1. CSV (.csv)\n"
              << "2. Text (.txt, tab-separated)\n"
              << "3. JSON (.json)\n";
    int format;
    if (!input::readInteger("Choose format: ", format) || format > 3)
    {
        std::cout << "Please select a format from 1 to 3.\n";
        return;
    }

    const Report report = loadReport(db, reportChoice);
    const std::string extension = format == 1 ? ".csv" : (format == 2 ? ".txt" : ".json");
    std::string fileName;
    if (!input::readText("Enter output file name (without extension): ", fileName))
    {
        return;
    }
    fileName += extension;

    std::error_code fileCheckError;
    if (std::filesystem::exists(fileName, fileCheckError))
    {
        std::cout << "That file already exists. Choose a different file name.\n";
        return;
    }
    if (fileCheckError)
    {
        std::cerr << "Could not check the output file: "
                  << fileCheckError.message() << '\n';
        return;
    }

    std::ofstream file(fileName);
    if (!file)
    {
        std::cerr << "Could not create export file: " << fileName << '\n';
        return;
    }

    if (format == 3)
    {
        file << "[\n";
        for (std::size_t rowIndex = 0; rowIndex < report.rows.size(); ++rowIndex)
        {
            file << "  {\n";
            for (std::size_t column = 0; column < report.headers.size(); ++column)
            {
                file << "    " << jsonValue(report.headers[column]) << ": "
                     << jsonValue(report.rows[rowIndex][column])
                     << (column + 1 == report.headers.size() ? "\n" : ",\n");
            }
            file << "  }" << (rowIndex + 1 == report.rows.size() ? "\n" : ",\n");
        }
        file << "]\n";
    }
    else
    {
        writeRow(file, report.headers, format);
        for (const auto &row : report.rows)
        {
            if (format == 1)
            {
                std::vector<std::string> escapedRow;
                for (const auto &value : row)
                {
                    escapedRow.push_back(csvValue(value));
                }
                writeRow(file, escapedRow, format);
            }
            else
            {
                writeRow(file, row, format);
            }
        }
    }

    if (!file)
    {
        std::cerr << "An error occurred while writing the export file.\n";
        return;
    }
    std::cout << "Exported " << report.rows.size() << " row(s) to "
              << fileName << ".\n";
}
