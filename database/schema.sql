CREATE DATABASE IF NOT EXISTS library_management
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE library_management;

CREATE TABLE IF NOT EXISTS books (
    book_id INT UNSIGNED PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    author VARCHAR(255) NOT NULL,
    total_copies INT UNSIGNED NOT NULL,
    available_copies INT UNSIGNED NOT NULL,
    CONSTRAINT chk_books_available_copies
        CHECK (available_copies <= total_copies)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS members (
    member_id INT UNSIGNED PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    contact VARCHAR(32) NOT NULL
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS book_issues (
    issue_id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    book_id INT UNSIGNED NOT NULL,
    member_id INT UNSIGNED NOT NULL,
    issue_date DATE NOT NULL,
    return_date DATE DEFAULT NULL,
    INDEX idx_book_issues_book_id (book_id),
    INDEX idx_book_issues_member_id (member_id),
    CONSTRAINT fk_book_issues_book
        FOREIGN KEY (book_id) REFERENCES books (book_id),
    CONSTRAINT fk_book_issues_member
        FOREIGN KEY (member_id) REFERENCES members (member_id),
    CONSTRAINT chk_book_issues_return_date
        CHECK (return_date IS NULL OR return_date >= issue_date)
) ENGINE=InnoDB;
