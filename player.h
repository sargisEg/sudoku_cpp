#ifndef PLAYER_H
#define PLAYER_H

#include <QString>


class Player
{
public:
    Player(const QString &name, const QString &difficluty, const QString &time);


    QString getName() const;
    QString getDifficluty() const;
    QString getTime() const;

private:
    QString name;
    QString difficluty;
    QString time;
};

#endif // PLAYER_H
