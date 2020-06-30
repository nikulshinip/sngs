#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "../controllers/dbadapter.h"
#include "../models/grouplistmodel.h"
#include "../models/grouplistdelegate.h"
#include "../models/dataproxymodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openBdAction_triggered();
    void groupCheckBoxClick();
    void plotWidgetGraphSelected(const QString &graphName);
    void groupListSelected(const QItemSelection &selected);

private:
    DbAdapter *dbAdapter;

    GroupListModel *groupListModel;
    GroupListDelegate *groupListDelegate;

    DataProxyModel *dataProxyModel;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
