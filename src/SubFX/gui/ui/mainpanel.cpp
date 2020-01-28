#include <memory>

#include "mainpanel.hpp"
#include "ui_mainpanel.h"

MainPanel *MainPanel::create(QWidget *parent)
{
    MainPanel *ret(new (std::nothrow) MainPanel(parent));
    if (!ret)
    {
        return nullptr;
    }

    Ui::MainPanel *ui(new (std::nothrow) Ui::MainPanel());
    if (!ui)
    {
        delete ret;
        return nullptr;
    }

    ret->m_ui = ui;
    ret->m_ui->setupUi(ret);

    return ret;
}

MainPanel::~MainPanel()
{
    if (m_ui)
    {
        delete m_ui;
    }
}

// public slots
void MainPanel::parseConfig(std::string &)
{}
