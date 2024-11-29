#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QWidget>

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

signals:
    void scores();
    void start(int difficulty, QString name);

private:
    Ui::WelcomeWindow *ui;

    void startButtonClick();
};

#endif // WELCOMEWINDOW_H
