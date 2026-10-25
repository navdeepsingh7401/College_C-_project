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
    registered_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT chk_books_available_copies
        CHECK (available_copies <= total_copies)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS members (
    member_id INT UNSIGNED PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    contact VARCHAR(32) NOT NULL,
    registered_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS book_issues (
    issue_id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    book_id INT UNSIGNED NOT NULL,
    member_id INT UNSIGNED NOT NULL,
    issue_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_book_issues_book_id (book_id),
    INDEX idx_book_issues_member_id (member_id),
    CONSTRAINT fk_book_issues_book
        FOREIGN KEY (book_id) REFERENCES books (book_id),
    CONSTRAINT fk_book_issues_member
        FOREIGN KEY (member_id) REFERENCES members (member_id)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS returned_books (
    return_id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    book_id INT UNSIGNED NOT NULL,
    member_id INT UNSIGNED NOT NULL,
    return_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_returned_books_book_id (book_id),
    INDEX idx_returned_books_member_id (member_id),
    CONSTRAINT fk_returned_books_book
        FOREIGN KEY (book_id) REFERENCES books (book_id),
    CONSTRAINT fk_returned_books_member
        FOREIGN KEY (member_id) REFERENCES members (member_id)
) ENGINE=InnoDB;

DROP TRIGGER IF EXISTS before_book_issue;
DROP TRIGGER IF EXISTS after_book_return;

DELIMITER //

CREATE TRIGGER before_book_issue
BEFORE INSERT ON book_issues
FOR EACH ROW
BEGIN
    UPDATE books
    SET available_copies = available_copies - 1
    WHERE book_id = NEW.book_id
      AND available_copies > 0;

    IF ROW_COUNT() = 0 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'No available copies for this book';
    END IF;
END//

CREATE TRIGGER after_book_return
AFTER INSERT ON returned_books
FOR EACH ROW
BEGIN
    UPDATE books
    SET available_copies = available_copies + 1
    WHERE book_id = NEW.book_id
      AND available_copies < total_copies;
END//

DELIMITER ;
