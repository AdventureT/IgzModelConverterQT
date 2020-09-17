#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath("");
    dirModel->setReadOnly(true);
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath("");
    fileModel->setNameFilters(QStringList("*igz"));
    fileModel->setNameFilterDisables(false);
    fileModel->setReadOnly(true);
    ui->treeView->setModel(dirModel);
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->treeView_2->setModel(fileModel);
    ui->treeView_2->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView_2->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_treeView_clicked(const QModelIndex &index)
{
    QString str = dirModel->fileInfo(index).absoluteFilePath();
    ui->treeView_2->setRootIndex(fileModel->setRootPath(str));
}

void Dialog::on_treeView_2_doubleClicked(const QModelIndex &index)
{
    //Start converting
    QString filePath = fileModel->fileInfo(index).filePath();
    //QMessageBox::information(this,"SAS", "SAS");
    IGZ igz(this, filePath.toStdString());
}
