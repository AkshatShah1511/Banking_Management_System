#include "thirdwindow.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include "secondwindow.h"
ThirdWindow::ThirdWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

ThirdWindow::~ThirdWindow() {
    // Cleanup if necessary
}

void ThirdWindow::setupUI() {
    // Create QLabels for name, age, and phone number
    QLabel *nameLabel = new QLabel("Name:", this);
    QLabel *ageLabel = new QLabel("Age:", this);
    QLabel *phoneLabel = new QLabel("Phone Number:", this);

    // Create QLineEdits for user input
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Enter your name");

    ageEdit = new QLineEdit(this);
    ageEdit->setPlaceholderText("Enter your age");

    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("Enter your phone number");

    // Create a submit button
    QPushButton *submitButton = new QPushButton("Submit", this);

    // Set styles for labels, line edits, and buttons
    QString labelStyle = "QLabel { font-size: 14px; font-weight: bold; color: #333; }";
    QString lineEditStyle = "QLineEdit { padding: 5px; font-size: 14px; border: 1px solid #ccc; border-radius: 4px; }";
    QString buttonStyle = "QPushButton { padding: 10px; font-size: 16px; background-color: #4CAF50; color: white; border: none; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }";

    nameLabel->setStyleSheet(labelStyle);
    ageLabel->setStyleSheet(labelStyle);
    phoneLabel->setStyleSheet(labelStyle);
    nameEdit->setStyleSheet(lineEditStyle);
    ageEdit->setStyleSheet(lineEditStyle);
    phoneEdit->setStyleSheet(lineEditStyle);
    submitButton->setStyleSheet(buttonStyle);

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(ageLabel);
    layout->addWidget(ageEdit);
    layout->addWidget(phoneLabel);
    layout->addWidget(phoneEdit);
    layout->addWidget(submitButton);

    setLayout(layout);

    // Connect the submit button to the slot
    connect(submitButton, &QPushButton::clicked, this, &ThirdWindow::handleSubmit);
}

void ThirdWindow::handleSubmit() {
    QString name = nameEdit->text();
    QString age = ageEdit->text();
    QString phone = phoneEdit->text();

    // Generate a 10-digit account number
    QString accountNumber = QString::number(QRandomGenerator::global()->bounded(qint64(1000000000), qint64(9999999999)));

    // Store the data in a file
    QFile file("userdata.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << "," << age << "," << phone << "," << accountNumber << "\n";
        file.close();
    }

    // Display the account number to the user (you can customize this part)
    QMessageBox::information(this, "Account Created", "Your account number is: " + accountNumber);

    // Switch to the second window
    SecondWindow *secondWindow = new SecondWindow();
    secondWindow->show();
    this->close();
}
