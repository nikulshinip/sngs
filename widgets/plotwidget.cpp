#include "plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent) : QCustomPlot(parent)
{
    setPlottingHint(QCP::phFastPolylines,true);

    QCPAxisTickerDateTime * dateTicker = new QCPAxisTickerDateTime();
    dateTicker->setDateTimeFormat("dd-MM-yyyy hh:mm:ss");
    dateTicker->setDateTimeSpec(Qt::LocalTime);
    xAxis->setTicker(QSharedPointer<QCPAxisTickerDateTime>(dateTicker));
    xAxis->setLabel("Дата, время");
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectPlottables | QCP::iSelectItems);
    axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
    xAxis->setSelectableParts(QCPAxis::spAxis);
    yAxis->setSelectableParts(QCPAxis::spAxis);
    addLayer("Tracers");

    connect(this, &QCustomPlot::selectionChangedByUser, this, &PlotWidget::selectionChange);
}

//=================================================================================================
void PlotWidget::init(GroupListModel *model)
{
    groupListModel = model;
    connect(groupListModel, &GroupListModel::modelReset, this, &PlotWidget::setTable);
    connect(groupListModel, &GroupListModel::changed,    this, &PlotWidget::parameterChange);
}

//=================================================================================================
void PlotWidget::selectionChange()
{
    QCPLayer * lyr = layer("Tracers");
    if (lyr!=nullptr){
        foreach(QCPLayerable*l, lyr->children()){
            QCPItemTracer * tr = static_cast<QCPItemTracer*>(l);
            if (tr == nullptr) continue;
            if (tr->selected()){
                if (tr->graph())
                    if (!tr->graph()->selected()){
                        tr->graph()->setSelection(QCPDataSelection(QCPDataRange(0,1)));
                        emit graphSelected(tr->graph()->name());
                    }
            }
        }
    }

    QCPAxis* cAx = nullptr;
    for(QCPAxis* ax : axisRect()->axes()){
        if (ax->selectedParts().testFlag(QCPAxis::spAxis))
            cAx = ax;
        if (ax->orientation()==Qt::Vertical)
            ax->grid()->setVisible(false);
    }

    if (cAx==nullptr){
        for (QCPGraph * gr : selectedGraphs()){
            cAx = gr->valueAxis();
            cAx->setSelectedParts(QCPAxis::spAxis);
            emit graphSelected(gr->name());
            break;
        }
    }

    if (cAx!=nullptr){
        axisRect()->setRangeZoomAxes(xAxis, cAx);
        axisRect()->setRangeDragAxes(xAxis, cAx);
        axisRect()->setRangeZoom(cAx->orientation());
        cAx->grid()->setVisible(true);
        return;
    }
    axisRect()->axis(QCPAxis::atLeft)->setVisible(true);
    axisRect()->setRangeZoomAxes(axisRect()->axes(QCPAxis::atBottom), axisRect()->axes(QCPAxis::atLeft));
    axisRect()->setRangeDragAxes(axisRect()->axes(QCPAxis::atBottom), axisRect()->axes(QCPAxis::atLeft));
    axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

//=================================================================================================
void PlotWidget::setTable()
{
    clearGraphs();
    for(int i = 0; i < groupListModel->groupList().count(); ++i){
        QCPGraph *tGraph = addGraph();
        tGraph->setName(QString::number(groupListModel->groupList().at(i)));

        QPen pen(groupListModel->colorsList().at(i), 2);
        tGraph->setPen(pen);

        QPen selectionDecorationPen(QColor(0, 0, 255), 4);
        tGraph->selectionDecorator()->setPen(selectionDecorationPen);

        QMap<qreal, qreal> values = DbAdapter::getData(groupListModel->table(), groupListModel->groupList().at(i));
        tGraph->setData(values.keys().toVector(), values.values().toVector());

        tGraph->setVisible(groupListModel->checkedsList().at(i));
    }
    rescaleAxes();
    replot();
}

//=================================================================================================
void PlotWidget::parameterChange(int groupListModelRow)
{
    int group = groupListModel->data(groupListModel->index(groupListModelRow)).toInt();
    QPen pen(groupListModel->data(groupListModel->index(groupListModelRow), Qt::DecorationRole).value<QColor>(), 2);
    graph(group)->setPen(pen);

    graph(group)->setVisible(groupListModel->data(groupListModel->index(groupListModelRow), Qt::CheckStateRole).toBool());
    replot(QCustomPlot::rpQueuedReplot);
}

//=================================================================================================
void PlotWidget::select(const QString &graphName)
{
    for(int i = 0; i < graphCount(); ++i){
        if(graph(i)->name() == graphName)
            graph(i)->setSelection(QCPDataSelection(graph(i)->data()->dataRange()));
        else
            graph(i)->setSelection(QCPDataSelection());
    }
    replot(QCustomPlot::rpQueuedReplot);
}
