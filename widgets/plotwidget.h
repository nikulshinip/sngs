#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "../libs/qcustomplot.h"
#include "../models/grouplistmodel.h"
#include "../controllers/dbadapter.h"

class PlotWidget : public QCustomPlot
{
    Q_OBJECT
public:
    PlotWidget(QWidget *parent = nullptr);

    void init(GroupListModel *model);

public slots:
    void selectionChange();
    void setTable();
    void parameterChange(int groupListModelRow);
    void select(const QString &graphName);

signals:
    void graphSelected(QString graphName);

private:
    GroupListModel *groupListModel;
};

#endif // PLOTWIDGET_H
