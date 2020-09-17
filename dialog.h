#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QDirModel>
#include <QFileSystemModel>
#include <QMessageBox>
#include "igz.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_2_doubleClicked(const QModelIndex &index);

private:
    Ui::Dialog *ui;
    QDirModel *model;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;
};
#endif // DIALOG_H
