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

    //Q_INVOKABLE void moveBall(int columnFrom, int rowFrom, int columnTo, int rowTo);
    Q_INVOKABLE void updateData();
    Q_INVOKABLE void forgetBall(int column, int row);

public slots:
    void addNewBall(int column, int row, int type);
    void setBall(int column, int row, int type);
    void markAsRemoveBall(int column, int row);

signals:
    void newGame();
    void moveBall(int columnFrom, int rowFrom, int columnTo, int rowTo);
    void scoreUpdated(unsigned int score);
    void gameOver();

private:
    Cell arrayCells2D[FIELD_SIDE_LENGTH][FIELD_SIDE_LENGTH];
};

#endif // GRIDITEMMODEL_H
