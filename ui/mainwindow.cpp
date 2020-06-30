#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbAdapter = new DbAdapter(this);
    ui->tablesComboBox->setModel(dbAdapter->getTableNamesModel());

    groupListModel = new GroupListModel(dbAdapter, this);
    groupListDelegate = new GroupListDelegate(this);
    ui->groupListView->setModel(groupListModel);
    ui->groupListView->setItemDelegate(groupListDelegate);

    dataProxyModel = new DataProxyModel(groupListModel, this);
    dataProxyModel->setSourceModel(dbAdapter->getDataModel());
    ui->dataTableView->setModel(dataProxyModel);

    ui->plotWidget->init(groupListModel);

    connect(ui->tablesComboBox, &QComboBox::currentTextChanged, groupListModel, &GroupListModel::setTable);
    connect(ui->tablesComboBox, &QComboBox::currentTextChanged, dbAdapter,      &DbAdapter::setTableIntoDataModel);
    connect(ui->groupCheckBox,  &QCheckBox::clicked,            this,           &MainWindow::groupCheckBoxClick);
    connect(groupListModel,     &GroupListModel::changed,       [&](){ ui->groupCheckBox->setCheckState(groupListModel->checksState()); });
    connect(ui->plotWidget,     &PlotWidget::graphSelected,     this,           &MainWindow::plotWidgetGraphSelected);
    connect(ui->groupListView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::groupListSelected);

    ui->statusbar->showMessage("Нет данных для отображения");
}

//=================================================================================================
//Файл -> открыть
void MainWindow::on_openBdAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть базу данных", QDir::homePath(), "*.db");
    if(!fileName.isEmpty()){
        if(!dbAdapter->connectDb(fileName)){
            QMessageBox::warning(this, "Ошибка", "Невозможно открыть файл базы данных");
        }else{
            ui->tablesComboBox->setCurrentIndex(0);
            ui->statusbar->showMessage("База данных открыта", 5000);
            ui->dataTableView->resizeColumnsToContents();
        }
    }
}

//=================================================================================================
void MainWindow::groupCheckBoxClick()
{
    Qt::CheckState state = ui->groupCheckBox->checkState();
    switch (state) {
        case Qt::Unchecked        : groupListModel->setAllCheckState(false); break;
        case Qt::PartiallyChecked : ui->groupCheckBox->setCheckState(Qt::Checked);
        case Qt::Checked          : groupListModel->setAllCheckState(true);
    }
}

//=================================================================================================
void MainWindow::plotWidgetGraphSelected(const QString &graphName)
{
    for(int i = 0; i < groupListModel->rowCount(); ++i){
        QModelIndex index = groupListModel->index(i);
        if(graphName == index.data().toString()){
            ui->groupListView->setCurrentIndex(index);
            return;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void MainWindow::groupListSelected(const QItemSelection &selected)
{
    if(!selected.isEmpty()){
        ui->plotWidget->select(selected.indexes().first().data().toString());
    }
}

//=================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
