#include "scoreswindow.h"
#include "ui_scoreswindow.h"

#include <QDebug>
#include <dbhelper.h>
#include <player.h>
#include <vector>

ScoresWindow::ScoresWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScoresWindow)
{
    ui->setupUi(this);
    this->move(200, 150);

}

ScoresWindow::~ScoresWindow()
{
    delete ui;
}

void ScoresWindow::initData()
{
    DbHelper dbHelper;
    std::vector<Player> players = dbHelper.getRecords();
    for (int i = 0; i < players.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(players[i].getName()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(players[i].getDifficluty()));
        auto timeStr = players[i].getTime();
        long time = timeStr.toLong();
        int sec = time / 1000;
        int min = sec / 60;
        sec = sec - (min * 60);
        int mil = time % 1000;
        QString msg;
        if (min < 10)
            msg.append("0");
        msg.append(QString::number(min)).append(":");
        if (sec < 10)
            msg.append("0");
        msg.append(QString::number(sec)).append(".").append(QString::number(mil));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(msg));
    }
}

void ScoresWindow::closeEvent(QCloseEvent *event)
{
    emit exit();
}
