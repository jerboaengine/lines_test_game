#include "agent.h"

Agent::Agent(QObject *parent)
    : QObject{parent}
{
    connect(&gameEngine, &Core::bornEvent, this, &Agent::bornEvent);
    connect(&gameEngine, &Core::deathEvent, this, &Agent::deathEvent);
    connect(&gameEngine, &Core::gameOverEvent, this, &Agent::gameOverEvent);
    connect(&gameEngine, &Core::gameWinEvent, this, &Agent::gameWinEvent);
    connect(&gameEngine, &Core::moveEvent, this, &Agent::moveEvent);
    connect(&gameEngine, &Core::updateScoreEvent, this, &Agent::updateScoreEvent);
}

bool Agent::moveRequest(int columnFrom, int rowFrom, int columnTo, int rowTo)
{
    return gameEngine.move(columnFrom, rowFrom, columnTo, rowTo);
}

void Agent::newGameRequest()
{
    gameEngine.newGame();
}

QList<QPoint> Agent::getAvailableCellsFor(unsigned int columnFrom, unsigned int rowFrom)
{
    return gameEngine.getAvailableCellsFor(columnFrom, rowFrom);
}

void Agent::restoreGameRequest()
{
    gameEngine.restoreGame();
}
