#ifndef SCORESWINDOW_H
#define SCORESWINDOW_H

#include <QWidget>

namespace Ui {
class ScoresWindow;
}

class ScoresWindow : public QWidget
{
    Q_OBJECT

signals:
    void exit();

public:
    explicit ScoresWindow(QWidget *parent = nullptr);
    ~ScoresWindow();
    void initData();

private:
    Ui::ScoresWindow *ui;


    void closeEvent(QCloseEvent *event);
};

#endif // SCORESWINDOW_H
