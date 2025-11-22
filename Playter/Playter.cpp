#include "Playter.h"
#include <QFileSystemModel>
#include <QFileInfo>
#include <QDir>

Playter::Playter(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    QFileSystemModel* model = new QFileSystemModel(this);
    model->setRootPath("");

    ui.File_List->setModel(model);
    ui.File_List->setRootIndex(model->index(""));

}

Playter::~Playter()
{}




//void Playter::on_BackButton_clicked()
//{
//    QModelIndex parentIndex = ui.File_List->rootIndex().parent();
//    if (parentIndex.isValid())
//        ui.File_List->setRootIndex(parentIndex);
//}