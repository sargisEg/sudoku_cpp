#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QElapsedTimer>
#include <QWidget>

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

signals:
    void exit();
    void solvedSignal();

public:
    explicit GameWindow(int difficulty, QString name, QWidget *parent = nullptr);
    ~GameWindow();

private:
    int currentRow;
    int currentCol;
    std::vector<std::vector<int>> board;
    int difficulty;
    QString name;
    QElapsedTimer timer;
    Ui::GameWindow *ui;

    void generateSolvedBoard();
    bool isValid(int, int, int);
    bool solve(int, int);
    void removeNumbers();
    void checkForSolution();
    int getRandomNumber();
    void makeDesignChanges();
    void writeInDb(QString time);

    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // GAMEWINDOW_H
