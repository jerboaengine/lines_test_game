#include "core.h"
#include <QRandomGenerator>

static constexpr int NUMBER_BALLS_PER_STEP = 3;
static constexpr int NUMBER_BALLS_SUCCESS = 5;
static constexpr int NUMBER_BALL_TYPES = 4;
static constexpr int BALL_SCORE_POINTS = 2;

Core::Core(QObject *parent)
    : QObject{parent}
{

}

void Core::newGame()
{
    source.clearGameFiled();
    notifyFieldUpdate();
    clearScore();
    generateNewBalls();
}

void Core::restoreGame()
{
    source.loadFromDB();
    notifyFieldUpdate();
    emit updateScoreEvent(source.getScore());
}

bool Core::generateNewBalls()
{
    for (int i = 0; i < NUMBER_BALLS_PER_STEP; i++) {
        int column;
        int row;
        int type;
        if (faindFreeRandomCell(column, row, type)) {
            emit bornEvent(column, row, type);
            source.setTypeCell(column, row, type);
        } else {
            return false;
        }
    }

    return true;
}

bool Core::faindFreeRandomCell(int &column, int &row, int &type)
{
    if (freeCellsCount() <= 0) {
        return false;
    }

    QRandomGenerator *rg = QRandomGenerator::global();

    while (true) {
        int randColumn = rg->bounded(source.columns());
        int randRow = rg->bounded(source.rows());
        if (source.getTypeCell(randColumn, randRow) == 0) {
            column = randColumn;
            row = randRow;
            type = rg->bounded(1, NUMBER_BALL_TYPES + 1);
            break;
        }
    }

    return true;
}

int Core::freeCellsCount() const
{
    int count = 0;
    for (unsigned int column = 0; column < source.columns(); column++) {
        for (unsigned int row = 0; row < source.rows(); row++) {
            if (source.getTypeCell(column, row) == 0) {
                count++;
            }
        }
    }

    return count;
}

void Core::gameOverHandler()
{
    emit gameOverEvent();
}

bool Core::findAndRemoveSequences()
{
    bool res = false;

    if (findAndRemoveHorizontalSequences()) {
        res = true;
    }

    if (findAndRemoveVerticalSequences()) {
        res = true;
    }

    return res;
}

bool Core::findAndRemoveHorizontalSequences()
{
    bool res = false;
    for (unsigned int row = 0; row < source.rows(); row++) {
        int lastType = -1;
        int firstPos = 0;
        int lastPos = 0;
        for (unsigned int column = 0; column < source.columns(); column++) {
            int type = source.getTypeCell(column, row);
            if (lastType != type) {
                if (lastType != 0 && removeHorizontalDiapason(row, firstPos, lastPos)) {
                    res = true;
                }
                lastType = type;
                firstPos = column;
            }

            lastPos = column;
        }
        if (lastType != 0) {
            if (removeHorizontalDiapason(row, firstPos, lastPos)) {
                res = true;
            }
        }
    }

    return res;
}

bool Core::findAndRemoveVerticalSequences()
{
    bool res = false;
    for (unsigned int column = 0; column < source.columns(); column++) {
        int lastType = -1;
        int firstPos = 0;
        int lastPos = 0;
        for (unsigned int row = 0; row < source.rows(); row++) {
            int type = source.getTypeCell(column, row);
            if (lastType != type) {
                if (lastType != 0 && removeVerticalDiapason(column, firstPos, lastPos)) {
                    res = true;
                }
                lastType = type;
                firstPos = row;
            }

            lastPos = row;
        }
        if (lastType != 0) {
            if (removeVerticalDiapason(column, firstPos, lastPos)) {
                res = true;
            }
        }
    }

    return res;
}

bool Core::removeHorizontalDiapason(unsigned int row, unsigned int firstPos, unsigned int lastPos)
{
    if ((lastPos - firstPos + 1) >= NUMBER_BALLS_SUCCESS) {
        for (unsigned int column = firstPos; column < lastPos + 1; column++) {
            source.setTypeCell(column, row, 0);
            emit deathEvent(column, row);
            addPoints();
        }
        return true;
    }
    return false;
}

bool Core::removeVerticalDiapason(unsigned int column, unsigned int firstPos, unsigned int lastPos)
{
    if ((lastPos - firstPos + 1) >= NUMBER_BALLS_SUCCESS) {
        for (unsigned int row = firstPos; row < lastPos + 1; row++) {
            source.setTypeCell(column, row, 0);
            emit deathEvent(column, row);
            addPoints();
        }
        return true;
    }
    return false;
}

bool Core::move(int columnFrom, int rowFrom, int columnTo, int rowTo)
{
    if (!isPossibleMove(columnFrom, rowFrom, columnTo, rowTo)) {
        return false;
    }

    source.swap(columnFrom, rowFrom, columnTo, rowTo);

    emit moveEvent(columnFrom, rowFrom, columnTo, rowTo);


    if (!findAndRemoveSequences()) {
        if (generateNewBalls()) {
            findAndRemoveSequences();
        } else {
            return true;
        }
    }

    if (freeCellsCount() < NUMBER_BALLS_PER_STEP) {
        gameOverHandler();
    }

    return true;
}

bool Core::isPossibleMove(int columnFrom, int rowFrom, int columnTo, int rowTo)
{
    if (source.getTypeCell(columnFrom, rowFrom) == EMPTY_CELL) {
        return false;
    }

    if (source.getTypeCell(columnTo, rowTo) != EMPTY_CELL) {
        return false;
    }

    return true;
}

void Core::addPoints()
{
    source.setScore(source.getScore() + BALL_SCORE_POINTS);
    emit updateScoreEvent(source.getScore());
}

void Core::clearScore()
{
    source.setScore(0);
    emit updateScoreEvent(source.getScore());
}

void Core::notifyFieldUpdate()
{
    for (unsigned int row = 0; row < source.rows(); row++) {
        for (unsigned int column = 0; column < source.columns(); column++) {
            int state = source.getTypeCell(column, row);
            if (state != EMPTY_CELL) {
                emit bornEvent(column, row, state);
            } else {
                emit deathEvent(column, row);
            }
        }
    }
}
