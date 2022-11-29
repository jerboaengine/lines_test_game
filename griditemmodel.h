#ifndef GRIDITEMMODEL_H
#define GRIDITEMMODEL_H

#include <QColor>
#include <QAbstractTableModel>
#include "gamecell.h"
#include "constants.h"

class GridItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    struct Cell : public GameCell {
        QString sourceType;
    };

    enum ItemRoles {
        TypeRole = Qt::UserRole + 1,
        SourceTypeRole,
        ColumnRole,
        RowRole
    };

    explicit GridItemModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex parent(const QModelIndex &) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void forgetBall(int column, int row);
    Q_INVOKABLE int getTypeBall(int column, int row);
    Q_INVOKABLE void removeBall(int column, int row);
    Q_INVOKABLE void insertBall(int column, int row, int type);

public slots:
    void addNewBall(int column, int row, int type);
    void markAsRemoveBall(int column, int row);

private:
    void updateCellData(int column, int row);

    Cell arrayCells2D[FIELD_SIDE_LENGTH][FIELD_SIDE_LENGTH];
};

#endif // GRIDITEMMODEL_H
