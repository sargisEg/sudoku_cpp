#include "dbhelper.h"
#include "gamewindow.h"
#include "qevent.h"
#include "ui_gamewindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <unordered_set>

GameWindow::GameWindow(int difficulty, QString name, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
{
    currentCol = 0;
    currentRow = 0;
    this->difficulty = difficulty;
    this->name = name;
    ui->setupUi(this);
    this->move(200, 150);
    generateSolvedBoard();
    removeNumbers();
    makeDesignChanges();
    timer.start();
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    QGridLayout *gridLayout = ui->gridLayout;

    if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_9) {
        int num = event->text().toInt();
        QLayoutItem *current = gridLayout->itemAtPosition(currentRow, currentCol);
        current->setAlignment(Qt::AlignCenter);
        QTextEdit *currentTextEdit = qobject_cast<QTextEdit *>(current->widget());
        QString type = currentTextEdit->property("type").toString();
        if (!type.compare("MISSING")) {
            QString str = " ";
            str.append(QString::number(num));
            currentTextEdit->setPlainText(str);
            int col = currentTextEdit->property("col").toInt();
            int row = currentTextEdit->property("row").toInt();
            board[row][col] = num;
            checkForSolution();
        }

    } else {
        int targetRow = currentRow;
        int targetCol = currentCol;
        switch (event->key()) {
        case Qt::Key_Up:
            targetRow = qMax(0, currentRow - 1);
            break;
        case Qt::Key_Down:
            targetRow = qMin(gridLayout->rowCount() - 1, currentRow + 1);
            break;
        case Qt::Key_Left:
            targetCol = qMax(0, currentCol - 1);
            break;
        case Qt::Key_Right:
            targetCol = qMin(gridLayout->columnCount() - 1, currentCol + 1);
            break;
        default:
            QWidget::keyPressEvent(event);
            return;
        }
        if(targetRow != currentRow || targetCol != currentCol) {
            QLayoutItem *current = gridLayout->itemAtPosition(currentRow, currentCol);
            QLayoutItem *target = gridLayout->itemAtPosition(targetRow, targetCol);
            target->widget()->setStyleSheet("QTextEdit{ border: 3px solid #FF9100; }");

            QTextEdit *currentTextEdit = qobject_cast<QTextEdit *>(current->widget());
            QString type = currentTextEdit->property("type").toString();

            if (!type.compare("MISSING")) {
                if(currentTextEdit->toPlainText().isEmpty()) {
                    current->widget()->setStyleSheet("QTextEdit{ border: 3px solid #690500; }");
                } else {
                    current->widget()->setStyleSheet("QTextEdit{ border: 3px solid #1c6900; }");
                }
            } else {
                current->widget()->setStyleSheet("QTextEdit{ border: 3px solid #934B00; }");
            }
            this->currentCol = targetCol;
            this->currentRow = targetRow;
        }
    }
}

void GameWindow::generateSolvedBoard()
{
    board = std::vector<std::vector<int>>(9, std::vector<int>(9, 0));
    solve(0, 0);
}

bool GameWindow::isValid(int row, int col, int num)
{
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

bool GameWindow::solve(int row, int col)
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::random_shuffle(numbers.begin(), numbers.end());
    if (row == 9) {
        return true;
    }

    if (col == 9) {
        return solve(row + 1, 0);
    }

    if (board[row][col] != 0) {
        return solve(row, col + 1);
    }

    for (int i = 0; i < numbers.size(); i++) {
        int num = numbers[i];
        if (isValid(row, col, num)) {
            board[row][col] = num;
            if (solve(row, col + 1)) {
                return true;
            }
            board[row][col] = 0;
        }
    }
    return false;
}

void GameWindow::removeNumbers()
{
    int count = 10;

    qInfo() << difficulty;
    if (difficulty == 0)
        count = 10;
    if (difficulty == 1)
        count = 20;
    if (difficulty == 2)
        count = 30;

    double col = getRandomNumber();
    double row = getRandomNumber();
    for (int i = 0; i < count; i++) {
        while (board[col][row] == 0) {
            col = getRandomNumber();
            row = getRandomNumber();
        }
        board[col][row] = 0;
    }
}

void GameWindow::checkForSolution()
{
    bool solved = true;

    for (int i = 0; i < 9; ++i) {
        std::unordered_set<int> rowSet;
        for (int j = 0; j < 9; ++j) {
            if (board[i][j] == 0) {
                solved = false;
                qInfo() << "Empty cell " << i << " " << j;
                break;
            }
            rowSet.insert(board[i][j]);
        }
        if (rowSet.size() != 9) {
            solved = false;
            qInfo() << "Duplicate in row " << i;

            break;
        }
    }

    if (solved)
        for (int j = 0; j < 9; ++j) {
            std::unordered_set<int> colSet;
            for (int i = 0; i < 9; ++i) {
                colSet.insert(board[i][j]);
            }
            if (colSet.size() != 9) {
                solved = false;
                qInfo() << "Duplicate in column " << j;
                break;
            }
        }

    if (solved)
        for (int block = 0; block < 9; block++) {
            std::unordered_set<int> subgridSet;
            int rowStart = (block / 3) * 3;
            int colStart = (block % 3) * 3;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (subgridSet.count(board[rowStart + i][colStart + j]) > 0) {
                        solved = false;
                        qInfo() << "Duplicate number in subgrid " << i << " " << j;
                        break;
                    }
                    subgridSet.insert(board[rowStart + i][colStart + j]);
                }
            }
        }

    if (solved) {
        long time = this->timer.elapsed();
        long sec = time / 1000;
        long min = sec / 60;
        sec = sec - (min * 60);
        long mil = time % 1000;
        QString minStr = QString::number(min);
        QString secStr = QString::number(sec);
        QString milStr = QString::number(mil);
        QString msg = "Congratulations! You solved the Sudoku in ";
        msg.append(minStr).append(" minutes ").append(secStr).append(" seconds ").append(milStr).append(" milliseconds.");
        QMessageBox::information(this, "Success", msg);
        writeInDb(QString::number(time));
        emit solvedSignal();
    }
}

void GameWindow::writeInDb(QString time) {

    QString diffName;
    switch (difficulty) {
    case 0:
        diffName = "Easy";
        break;
    case 1:
        diffName = "Medium";
        break;
    case 2:
        diffName = "Hard";
        break;
    }

    DbHelper dbHelper;
    dbHelper.addRecord(name, diffName, time);

}

int GameWindow::getRandomNumber()
{
    return QRandomGenerator::global()->bounded(1, 9);
}

void GameWindow::makeDesignChanges()
{
    QGridLayout *gridLayout = ui->gridLayout;

    for (int row = 0; row < gridLayout->rowCount(); row++) {
        for (int col = 0; col < gridLayout->columnCount(); col++) {
            QLayoutItem *item = gridLayout->itemAtPosition(row, col);
            if (item) {
                QTextEdit *textEdit = qobject_cast<QTextEdit *>(item->widget());
                textEdit->setProperty("col", col);
                textEdit->setProperty("row", row);
                int number = board[row][col];
                if (number == 0) {
                    textEdit->setStyleSheet("QTextEdit{ border: 3px solid #690500; }");
                    textEdit->setProperty("type", "MISSING");
                } else {
                    QString str = " ";
                    str.append(QString::number(number));
                    textEdit->setPlainText(str);
                    textEdit->setProperty("type", "NOT_MISSING");
                }
            }
        }
    }
}

void GameWindow::closeEvent(QCloseEvent *event)
{
    emit exit();
}
