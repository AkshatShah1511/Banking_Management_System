#ifndef THIRDWINDOW_H
#define THIRDWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class ThirdWindow : public QWidget {
    Q_OBJECT

public:
    explicit ThirdWindow(QWidget *parent = nullptr);
    ~ThirdWindow();

private slots:
    void handleSubmit();

private:
    void setupUI();

    QLineEdit *nameEdit;
    QLineEdit *ageEdit;
    QLineEdit *phoneEdit;
};

#endif // THIRDWINDOW_H
