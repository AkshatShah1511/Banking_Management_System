#include "secondwindow.h"
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include "forthwindow.h"

SecondWindow::SecondWindow(QWidget *parent) : QMainWindow(parent), attempts(0) {
    setupUI();
}

SecondWindow::~SecondWindow() {
    // Destructor (not doing anything special here, but you can add custom cleanup if needed)
}

void SecondWindow::setupUI() {
    // Set background color for the window
    this->setStyleSheet("QMainWindow { background-color: #f0f0f0; }");

    // Create UI elements for name and account number inputs
    QLabel *nameLabel = new QLabel("Enter your name:", this);
    nameLabel->setStyleSheet("QLabel { font-size: 18px; color: #333; padding: 5px; }");

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Name");
    nameInput->setStyleSheet("QLineEdit { padding: 10px; font-size: 16px; border: 1px solid #ccc; border-radius: 5px; }");

    QLabel *accountNumberLabel = new QLabel("Enter your account number:", this);
    accountNumberLabel->setStyleSheet("QLabel { font-size: 18px; color: #333; padding: 5px; }");

    accountNumberInput = new QLineEdit(this);
    accountNumberInput->setPlaceholderText("Account Number");
    accountNumberInput->setStyleSheet("QLineEdit { padding: 10px; font-size: 16px; border: 1px solid #ccc; border-radius: 5px; }");

    // Create and style the submit button
    QPushButton *submitBtn = new QPushButton("Login", this);
    submitBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-size: 18px; padding: 12px; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");

    // Layout setup: Using QFormLayout to align labels and inputs neatly
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(nameLabel, nameInput);
    formLayout->addRow(accountNumberLabel, accountNumberInput);
    formLayout->setVerticalSpacing(15); // Add spacing between form rows

    // Add submit button to a vertical layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addSpacing(20); // Add spacing between inputs and button
    layout->addWidget(submitBtn, 0, Qt::AlignCenter); // Center the button

    // Set central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect the button click to the slot for handling account creation
    connect(submitBtn, &QPushButton::clicked, this, &SecondWindow::oncreateAccount);
}

void SecondWindow::saveAccountToFile(const QString &name, const QString &accountNumber) {
    QFile file("accounts.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << "\n" << accountNumber << "\n";
        file.close();
    } else {
        QMessageBox::warning(this, "File Error", "Unable to open the accounts file.");
    }
}

void SecondWindow::oncreateAccount() {
    QString name = nameInput->text();
    QString accountNumber = accountNumberInput->text();

    if (name.isEmpty() || accountNumber.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both name and account number.");
        return;
    }

    if (checkCredentials(name, accountNumber)) {
        // Create the ForthWindow and pass account details
        ForthWindow *forthwindow = new ForthWindow();
        forthwindow->setAccountDetails(accountNumber, name);  // Pass account number and name
        forthwindow->show();
        this->close();
    } else {
        attempts++;
        if (attempts >= 3) {
            QMessageBox::critical(this, "Login Failed", "Maximum attempts reached. Exiting.");
            QCoreApplication::exit();
        } else {
            QMessageBox::warning(this, "Login Failed", "Incorrect name or account number. Attempts left: " + QString::number(3 - attempts));
        }
    }
}

bool SecondWindow::checkCredentials(const QString &name, const QString &accountNumber) {
    QFile file("userdata.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 4 && parts[0] == name && parts[3] == accountNumber) {
                return true;
            }
        }
        file.close();
    }
    return false;
}
