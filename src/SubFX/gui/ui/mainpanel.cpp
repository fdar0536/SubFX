#include <memory>

#include "mainpanel.hpp"
#include "ui_mainpanel.h"
#undef B0

#include "QThreadPool"

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
    ret->connectHook();

    return ret;
}

MainPanel::~MainPanel()
{
    if (QThreadPool::globalInstance()->activeThreadCount())
            QThreadPool::globalInstance()->waitForDone();

    if (m_ui)
    {
        delete m_ui;
    }

    if (m_assInitBg)
    {
        delete m_assInitBg;
    }
}

// public member functions
void MainPanel::parseConfig(std::string &jsonFileName)
{
    std::string tmpString(jsonFileName);
    m_assInitBg->setJsonFileName(tmpString);
    QThreadPool::globalInstance()->start(m_assInitBg);
}

// private member function
void MainPanel::connectHook()
{
    // message
    connect(m_assInitBg,
            SIGNAL(message(QString &)),
            this,
            SIGNAL(message(QString &)));
}
