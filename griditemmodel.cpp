#include "griditemmodel.h"
#include <QString>

static constexpr auto BORN_SOURCE = "born";
static constexpr auto REMOVE_SOURCE = "remove";

GridItemModel::GridItemModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

int GridItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return FIELD_SIZE;
}

int GridItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 0;
}

QModelIndex GridItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return row >= 0 && row < FIELD_SIZE && column == 0? createIndex(row, column) : QModelIndex();
}

QVariant GridItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= FIELD_SIZE) {
       return QVariant();
    }

    const Cell *arrayPtr = &arrayCells2D[0][0];
    const Cell &cell = arrayPtr[index.row()];
    switch (role) {
        case TypeRole:
            return cell.type;
        case SourceTypeRole:
            return cell.sourceType;
        case ColumnRole:
            return index.row() / FIELD_SIDE_LENGTH;
        case RowRole:
            return index.row() % FIELD_SIDE_LENGTH;
        case AvailableToStepRole:
            return cell.isAvailableToStep;
        default:
            break;
    }

    return QVariant();
}

QModelIndex GridItemModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QHash<int, QByteArray> GridItemModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[SourceTypeRole] = "source";
    roles[ColumnRole] = "column";
    roles[RowRole] = "row";
    roles[AvailableToStepRole] = "available";
    return roles;
}

void GridItemModel::addNewBall(int column, int row, int type)
{
    arrayCells2D[column][row].type = type;
    arrayCells2D[column][row].sourceType = BORN_SOURCE;
    updateCellData(column, row);
}

void GridItemModel::markAsRemoveBall(int column, int row)
{
    arrayCells2D[column][row].sourceType = REMOVE_SOURCE;
    updateCellData(column, row);
}

void GridItemModel::forgetBall(int column, int row)
{
    Cell &cell = arrayCells2D[column][row];
    if (cell.sourceType == REMOVE_SOURCE) {
        cell.sourceType.clear();
        cell.type = 0;

        updateCellData(column, row);
    }
}

int GridItemModel::getTypeBall(int column, int row)
{
    return arrayCells2D[column][row].type;
}

void GridItemModel::removeBall(int column, int row)
{
    arrayCells2D[column][row].type = EMPTY_CELL;
    arrayCells2D[column][row].sourceType.clear();
    updateCellData(column, row);
}

void GridItemModel::insertBall(int column, int row, int type)
{
    arrayCells2D[column][row].type = type;
    updateCellData(column, row);
}

void GridItemModel::setAvailableCells(QList<QPoint> cells)
{
    clearAvailableCells();
    for (const QPoint &cell : cells) {
        arrayCells2D[cell.x()][cell.y()].isAvailableToStep = true;
        updateCellData(cell.x(), cell.y());
    }
}

void GridItemModel::clearAvailableCells()
{
    for (unsigned int row = 0; row < FIELD_SIDE_LENGTH; row++) {
        for (unsigned int column = 0; column < FIELD_SIDE_LENGTH; column++) {
            Cell &cell = arrayCells2D[column][row];
            if (cell.isAvailableToStep) {
                cell.isAvailableToStep = false;
                updateCellData(column, row);
            }
        }
    }
}

void GridItemModel::updateCellData(int column, int row)
{
    const int indexItem = FIELD_SIDE_LENGTH * column + row;
    QModelIndex topLeft = createIndex(indexItem, 0);
    QModelIndex bottomRight = createIndex(indexItem, 0);
    emit dataChanged(topLeft, bottomRight);
}
