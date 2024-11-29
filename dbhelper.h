#ifndef DBHELPER_H
#define DBHELPER_H

#include <QSqlDatabase>
#include <QString>
#include <player.h>
#include <vector>


class DbHelper
{
public:
    DbHelper();
    ~DbHelper();

    void addRecord(QString name, QString difficulty, QString time);
    std::vector<Player> getRecords();

private:
    QSqlDatabase db;
};

#endif // DBHELPER_H
