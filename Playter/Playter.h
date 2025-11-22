#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Playter.h"
#include <QFileSystemModel>

class Playter : public QMainWindow
{
    Q_OBJECT

public:
    Playter(QWidget *parent = nullptr);
    ~Playter();

private slots:
private:
    Ui::PlayterClass ui;
    QFileSystemModel* model;
};


