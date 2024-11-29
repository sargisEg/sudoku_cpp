#include "player.h"

Player::Player(const QString &name, const QString &difficluty, const QString &time) : name(name),
    difficluty(difficluty),
    time(time)
{}

QString Player::getName() const
{
    return name;
}

QString Player::getDifficluty() const
{
    return difficluty;
}

QString Player::getTime() const
{
    return time;
}
