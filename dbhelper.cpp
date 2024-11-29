#include "dbhelper.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <vector>

DbHelper::DbHelper() {

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("rAyWMlPeVsAs4Du");
    if (!db.open())
    {
        qDebug() << "Failed to connect to root mysql admin";
    }
}

DbHelper::~DbHelper()
{
    db.close();
}

void DbHelper::addRecord(QString name, QString difficulty, QString time)
{
    QSqlQuery query;
    QString queryMsg = "insert into players (name, difficulty, score) values ('";
    queryMsg.append(name).append("', '").append(difficulty).append("', ").append(time).append(")");
    bool queryRes = query.exec(queryMsg);
    if (query.lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query.lastError().text();
    }

}

std::vector<Player> DbHelper::getRecords()
{

    std::vector<Player> players;

    QSqlQuery query;
    bool queryRes = query.exec("SELECT name, difficulty, score FROM players order by score asc");
    if (query.lastError().type() != QSqlError::NoError || !queryRes)
    {
        qDebug() << query.lastError().text();
    } else {
        while (query.next())
        {
            players.push_back(Player(query.value(0).toString(), query.value(1).toString(), query.value(2).toString()));
        }

    }

    return players;
}
