#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QLineEdit>

class SecondWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void oncreateAccount();


private:
    void setupUI();
    void saveAccountToFile(const QString &name, const QString &accountNumber);
    bool checkCredentials(const QString &name, const QString &accountNumber);

    QLineEdit *nameInput;
    QLineEdit *accountNumberInput;
    int attempts;
};

#endif // SECONDWINDOW_H
