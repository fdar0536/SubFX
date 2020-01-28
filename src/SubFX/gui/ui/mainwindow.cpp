#include <memory>

#include "QMessageBox"
#include "QFileDialog"
#include "QDir"

#include "config.h"
#include "../../common/basecommon.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow *MainWindow::create(QWidget *parent)
{
    MainWindow *ret(new (std::nothrow) MainWindow(parent));
    if (!ret)
    {
        return nullptr;
    }

    Ui::MainWindow *ui(new (std::nothrow) Ui::MainWindow());
    if (!ui)
    {
        delete ret;
        return nullptr;
    }

    ret->m_ui = ui;
    ret->m_ui->setupUi(ret);
    int err(ret->init());
    if (err)
    {
        delete ret;
        return nullptr;
    }

    return ret;
}

MainWindow::~MainWindow()
{
    if (m_ui)
    {
        delete m_ui;
    }

    if (m_mainPanel)
    {
        delete m_mainPanel;
    }

    if (m_logPanel)
    {
        delete m_logPanel;
    }
}

// private slots
void MainWindow::onOpenTriggled(bool)
{
    QString res(QFileDialog::getOpenFileName(this,
                                             tr("Open config file"),
                                             QDir::homePath(),
                                             tr("JSON (*.json)")
                ));
    if (res == "")
    {
        return;
    }
}

void MainWindow::onAboutQtTriggled(bool)
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onExitTriggled(bool)
{
    handleExit();
}

// private member function
int MainWindow::init()
{
    m_mainPanel = MainPanel::create(this);
    if (!m_mainPanel)
    {
        return 1;
    }
    m_ui->mainTab->insertTab(0, m_mainPanel, "Main");

    m_logPanel = LogPanel::create(this);
    if (!m_logPanel)
    {
        return 1;
    }
    m_ui->mainTab->insertTab(1, m_logPanel, "Log");

    m_title = SUBFX_NAME;
    m_title += " GUI " + QString(SUBFX_VERSION);
    setWindowTitle(m_title);

    connectHook();
    return 0;
}

void MainWindow::connectHook()
{
    // menu bar
    connect(m_ui->actionOpen,
            SIGNAL(triggered(bool)),
            this,
            SLOT(onOpenTriggled(bool)));

    connect(m_ui->actionAboutQt,
            SIGNAL(triggered(bool)),
            this,
            SLOT(onAboutQtTriggled(bool)));

    connect(m_ui->actionExit,
            SIGNAL(triggered(bool)),
            this,
            SLOT(onExitTriggled(bool)));
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    e->ignore();
    handleExit();
}

void MainWindow::handleExit()
{
    QMessageBox::StandardButton res(QMessageBox::question(
        this,
        tr("Exit"),
        tr("Are you sure to exit?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    ));

    if (res == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}
