#include "forthWindow.h"
#include "secondwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QDialog>
#include <QFormLayout>
#include <QDateTime>

ForthWindow::ForthWindow(QWidget *parent) : QWidget(parent), balance(0.0) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setStyleSheet("background-color: #f0f0f0; color: #333; font-family: Arial, sans-serif;");

    // Deposit layout
    QHBoxLayout *depositLayout = new QHBoxLayout();
    depositEdit = new QLineEdit(this);
    depositEdit->setPlaceholderText("Enter deposit amount");
    depositEdit->setStyleSheet("background-color: white; color: black; border: 2px solid #00796b; border-radius: 5px; padding: 10px;");
    depositEdit->setMinimumHeight(40);
    depositEdit->setMinimumWidth(200);
    QPushButton *depositButton = new QPushButton("Deposit", this);
    depositButton->setStyleSheet("background-color: #00796b; color: white; border: none; border-radius: 5px; padding: 10px;");
    depositButton->setMinimumSize(120, 40);
    depositLayout->addWidget(depositEdit);
    depositLayout->addWidget(depositButton);
    connect(depositButton, &QPushButton::clicked, this, &ForthWindow::onDepositClicked);

    // Withdraw layout
    QHBoxLayout *withdrawLayout = new QHBoxLayout();
    withdrawEdit = new QLineEdit(this);
    withdrawEdit->setPlaceholderText("Enter withdrawal amount");
    withdrawEdit->setStyleSheet("background-color: white; color: black; border: 2px solid #00796b; border-radius: 5px; padding: 10px;");
    withdrawEdit->setMinimumHeight(40);
    withdrawEdit->setMinimumWidth(200);
    QPushButton *withdrawButton = new QPushButton("Withdraw", this);
    withdrawButton->setStyleSheet("background-color: #00796b; color: white; border: none; border-radius: 5px; padding: 10px;");
    withdrawButton->setMinimumSize(120, 40);
    withdrawLayout->addWidget(withdrawEdit);
    withdrawLayout->addWidget(withdrawButton);
    connect(withdrawButton, &QPushButton::clicked, this, &ForthWindow::onWithdrawClicked);

    // Add layouts to main layout
    mainLayout->addLayout(depositLayout);
    mainLayout->addLayout(withdrawLayout);

    // Check Balance button
    QPushButton *checkBalanceButton = new QPushButton("Check Balance", this);
    checkBalanceButton->setStyleSheet("background-color: #004d40; color: white; border: none; border-radius: 5px; padding: 15px;");
    checkBalanceButton->setMinimumSize(200, 50); // Larger button size
    connect(checkBalanceButton, &QPushButton::clicked, this, &ForthWindow::onCheckBalanceClicked);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("margin-top: 10px;");
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(checkBalanceButton);

    // Profile Button
    QPushButton *viewProfileButton = new QPushButton("View Profile", this);
    viewProfileButton->setStyleSheet("background-color: #004d40; color: white; border: none; border-radius: 5px; padding: 10px;");
    viewProfileButton->setMinimumSize(200, 40);
    connect(viewProfileButton, &QPushButton::clicked, this, &ForthWindow::viewProfile);
    mainLayout->addWidget(viewProfileButton);

    // Transaction History Button
    QPushButton *transactionHistoryButton = new QPushButton("Transaction History", this);
    transactionHistoryButton->setStyleSheet("background-color: #004d40; color: white; border: none; border-radius: 5px; padding: 10px;");
    transactionHistoryButton->setMinimumSize(200, 40);
    connect(transactionHistoryButton, &QPushButton::clicked, this, &ForthWindow::viewTransactionHistory);
    mainLayout->addWidget(transactionHistoryButton);

    // Logout Button
    QPushButton *logoutButton = new QPushButton("Logout", this);
    logoutButton->setStyleSheet("background-color: #004d40; color: white; border: none; border-radius: 5px; padding: 10px;");
    logoutButton->setMinimumSize(200, 40);
    connect(logoutButton, &QPushButton::clicked, this, &ForthWindow::logout);
    mainLayout->addWidget(logoutButton);

    // Add a spacer to push buttons to the bottom
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addItem(spacer);

    setLayout(mainLayout);
}

void ForthWindow::onDepositClicked() {
    bool ok;
    double depositAmount = depositEdit->text().toDouble(&ok);
    if (ok && depositAmount > 0) {
        balance += depositAmount;
        saveBalance();  // Save updated balance
        logTransaction("Deposit", depositAmount);  // Log the deposit
        statusLabel->setText("Deposited: $" + QString::number(depositAmount));
        depositEdit->clear();
    } else {
        QMessageBox::warning(this, "Input Error", "Please enter a valid deposit amount.");
    }
}

void ForthWindow::onWithdrawClicked() {
    bool ok;
    double withdrawAmount = withdrawEdit->text().toDouble(&ok);
    if (ok && withdrawAmount > 0 && withdrawAmount <= balance) {
        balance -= withdrawAmount;
        saveBalance();  // Save updated balance
        logTransaction("Withdraw", withdrawAmount);  // Log the withdrawal
        statusLabel->setText("Withdrew: $" + QString::number(withdrawAmount));
        withdrawEdit->clear();
    } else {
        QMessageBox::warning(this, "Input Error", "Please enter a valid withdrawal amount.");
    }
}

void ForthWindow::onCheckBalanceClicked()
{
    QMessageBox::information(this, "Account Balance", QString("Your balance is: $%1").arg(balance, 0, 'f', 2));
    statusLabel->setText("Balance checked.");
}

// Function to load account data from the file
void ForthWindow::loadAccountData()
{
    QFile file("accounts.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        bool accountFound = false;

        // Read line by line to find the account
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if (fields.size() == 3 && fields[0] == accountNumber) {
                name = fields[1];
                balance = fields[2].toDouble();
                accountFound = true;
                break;
            }
        }
        file.close();

        if (!accountFound) {
            balance = 0.0;
            statusLabel->setText("No existing account found. Starting with zero balance.");
        } else {
            statusLabel->setText(QString("Account loaded. Balance: $%1").arg(balance, 0, 'f', 2));
        }
    } else {
        statusLabel->setText("Failed to load account data.");
    }
}

// Function to save account data to the file
void ForthWindow::saveAccountData()
{
    QFile file("accounts.txt");
    QStringList lines;
    bool accountFound = false;

    // Read the current file content
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if (fields.size() == 3 && fields[0] == accountNumber) {
                lines.append(QString("%1,%2,%3").arg(accountNumber).arg(name).arg(balance));
                accountFound = true;
            } else {
                lines.append(line);
            }
        }
        file.close();
    }

    if (!accountFound) {
        lines.append(QString("%1,%2,%3").arg(accountNumber).arg(name).arg(balance));
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString &line : lines) {
            out << line << '\n';
        }
        file.close();
    } else {
        statusLabel->setText("Failed to save account data.");
    }
}
void ForthWindow::setAccountDetails(const QString &accountNumber, const QString &name) {
    this->accountNumber = accountNumber;
    this->name = name;
    loadAccountData();  // Load the balance from the file associated with this account number
}
void ForthWindow::viewProfile() {
    QDialog *profileDialog = new QDialog(this);
    profileDialog->setWindowTitle("Profile Information");
    QFormLayout *formLayout = new QFormLayout(profileDialog);

    // Add labels to show account details
    QLabel *nameLabel = new QLabel("Name: " + name, profileDialog);
    QLabel *accountNumberLabel = new QLabel("Account Number: " + accountNumber, profileDialog);

    formLayout->addRow(nameLabel);
    formLayout->addRow(accountNumberLabel);

    // Create a "Close" button
    QPushButton *closeButton = new QPushButton("Close", profileDialog);
    connect(closeButton, &QPushButton::clicked, profileDialog, &QDialog::accept);
    formLayout->addWidget(closeButton);

    profileDialog->setLayout(formLayout);
    profileDialog->exec();
}
void ForthWindow::logTransaction(const QString &type, double amount) {
    QFile file("transactions.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        out << accountNumber << "," << type << "," << amount << "," << timestamp << "\n";
        file.close();
    } else {
        QMessageBox::warning(this, "File Error", "Unable to open transactions file.");
    }
}

// View Transaction History function
void ForthWindow::viewTransactionHistory() {
    QFile file("transactions.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString history;

        // Load transaction history for this account
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 4 && parts[0] == accountNumber) {  // Ensure correct format and filter by account number
                QString type = parts[1];
                QString amount = parts[2];
                QString timestamp = parts[3];
                history += type + ": $" + amount + " on " + timestamp + "\n";
            }
        }
        file.close();

        // Show transaction history in a message box
        if (history.isEmpty()) {
            history = "No transactions found for your account.";
        }
        QMessageBox::information(this, "Transaction History", history);
    } else {
        QMessageBox::warning(this, "File Error", "Unable to open transaction file.");
    }
}

// Logout function
void ForthWindow::logout() {
    // Close the current window and return to the login window
    SecondWindow *loginWindow = new SecondWindow();
    loginWindow->show();
    this->close();
}
void ForthWindow::saveBalance() {
    QFile file("userdata.txt");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList lines;

        // Read existing file content
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.seek(0);  // Go back to the start of the file to overwrite

        // Update balance for the current account
        for (int i = 0; i < lines.size(); i++) {
            QStringList parts = lines[i].split(",");
            if (parts.size() == 4 && parts[3] == accountNumber) {  // Match account number
                lines[i] = parts[0] + "," + parts[1] + "," + QString::number(balance) + "," + parts[3];  // Update balance
            }
        }

        // Write back updated content to the file
        for (const QString &line : lines) {
            in << line << "\n";
        }
        file.close();
    } else {
        QMessageBox::warning(this, "File Error", "Unable to open userdata file.");
    }
}
void ForthWindow::loadBalance() {
    QFile file("userdata.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 4 && parts[3] == accountNumber) {  // Match account number
                balance = parts[2].toDouble();  // Load balance
                break;
            }
        }
        file.close();
    } else {
        QMessageBox::warning(this, "File Error", "Unable to open userdata file.");
    }
}
