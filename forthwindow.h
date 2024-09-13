#ifndef FORTHWINDOW_H
#define FORTHWINDOW_H

#include <QWidget>
#include <QStringList>

class QLineEdit;
class QPushButton;
class QLabel;

class ForthWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ForthWindow(QWidget *parent = nullptr);

    void setAccountDetails(const QString &accountNumber, const QString &name);

private slots:
    void onDepositClicked();
    void onWithdrawClicked();
    void onCheckBalanceClicked();

private:
    void loadAccountData();
    void saveAccountData();
    void addTransaction(const QString &type, double amount);
    void viewTransactionHistory();
    void updateProfileView();  // New method
    void viewProfile();
    void logout();
    void logTransaction(const QString &type, double amount);
    void saveBalance() ;
    void loadBalance();
    QLineEdit *depositEdit;
    QLineEdit *withdrawEdit;
    QLabel *statusLabel;
    QLabel *accountNumberLabel; // New QLabel for account number
    QLabel *nameLabel;          // New QLabel for name
    QPushButton *checkBalanceButton;

    QString accountNumber;
    QString name;
    double balance;
    QStringList transactions;  // Store transaction history
};

#endif // FORTHWINDOW_H
