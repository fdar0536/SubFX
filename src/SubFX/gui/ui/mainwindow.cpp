#include <new>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow *MainWindow::create(QWidget *parent)
{
    MainWindow *ret(new (std::nothrow) MainWindow(parent));
    if (!ret)
    {
        return nullptr;
    }
    ret->m_ui = new (std::nothrow) Ui::MainWindow();
    if (!ret->m_ui)
    {
        delete ret;
        return nullptr;
    }

    ret->m_ui->setupUi(ret);
    return ret;
}

MainWindow::~MainWindow()
{
    if (m_ui) delete m_ui;
}
