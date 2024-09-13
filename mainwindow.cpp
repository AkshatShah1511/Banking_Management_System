#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "thirdwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Set main window background color
    this->setStyleSheet("QMainWindow { background-color: #f5f5f5; }");

    // Central widget to hold all layouts
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a vertical layout for the central widget
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 1. Logo at the top (centered)
    QPixmap logoPixmap("/Users/akshat/Downloads/be3d29ae7fe59c3ed84f1cc9393a2ad8.jpg");
    QLabel *logoLabel = new QLabel(this);
    logoLabel->setPixmap(logoPixmap.scaled(150, 150, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter); // Center the logo
    mainLayout->addWidget(logoLabel);

    // Add a small space between logo and welcome message
    mainLayout->addSpacing(20);

    // 2. Welcome message (centered)
    QLabel *welcomeLabel = new QLabel("Welcome to YourBank!", this);
    welcomeLabel->setAlignment(Qt::AlignCenter); // Center the text
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    mainLayout->addWidget(welcomeLabel);

    // Add a larger space between welcome message and buttons
    mainLayout->addStretch(1);

    // 3. Create horizontal layout for the buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    // Create Account button
    QPushButton *createAccountButton = new QPushButton("Create Account", this);
    createAccountButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-size: 18px; padding: 10px 20px; border-radius: 5px; } QPushButton:hover { background-color: #45a049; }");
    buttonLayout->addWidget(createAccountButton);

    // Add a spacer between the two buttons
    buttonLayout->addSpacing(40);

    // Log In button
    QPushButton *logInButton = new QPushButton("Log In", this);
    logInButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-size: 18px; padding: 10px 20px; border-radius: 5px; } QPushButton:hover { background-color: #1976D2; }");
    buttonLayout->addWidget(logInButton);

    // Center the button layout by adding it to the vertical layout
    mainLayout->addLayout(buttonLayout);

    // Add a stretch at the bottom to push everything upwards and give space at the bottom
    mainLayout->addStretch(1);

    // Set up signals for button clicks (navigate to next windows)
    connect(createAccountButton, &QPushButton::clicked, this, &MainWindow::on_createAccountButton_clicked);
    connect(logInButton, &QPushButton::clicked, this, &MainWindow::on_logInButton_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_createAccountButton_clicked() {
    this->hide();
    ThirdWindow *thirdwindow = new ThirdWindow();
    thirdwindow->show();
    this->close();
}

void MainWindow::on_logInButton_clicked() {
    this->hide();
    SecondWindow *secondWindow = new SecondWindow();
    secondWindow->show();
    this->close();
}



