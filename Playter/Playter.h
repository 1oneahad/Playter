#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Playter.h"

class Playter : public QMainWindow
{
    Q_OBJECT

public:
    Playter(QWidget *parent = nullptr);
    ~Playter();

private:
    Ui::PlayterClass ui;
};

