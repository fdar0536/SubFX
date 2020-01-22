#include <memory>

#include "QMessageBox"

#include "config.h"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

std::pair<MainWindow *, const char *>
MainWindow::create(QWidget *parent)
{
    MainWindow *ret(new (std::nothrow) MainWindow(parent));
    if (!ret)
    {
        return std::make_pair(nullptr,
                              "Fail to allocate memory");
    }

    Ui::MainWindow *ui(new (std::nothrow) Ui::MainWindow());
    if (!ui)
    {
        delete ret;
        return std::make_pair(nullptr,
                              "Fail to allocate memory");
    }

    ret->m_ui = ui;
    ret->m_ui->setupUi(ret);
    const char *err(ret->init());
    if (err)
    {
        delete ret;
        return std::make_pair(nullptr, err);
    }

    return std::make_pair(ret, nullptr);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

// private slots
void MainWindow::onAboutQtTriggled(bool)
{
    QMessageBox::aboutQt(this);
}

void MainWindow::onExitTriggled(bool)
{
    handleExit();
}

// private member function
const char *MainWindow::init()
{
    m_title = SUBFX_NAME;
    m_title += " GUI " + QString(SUBFX_VERSION);
    setWindowTitle(m_title);
    connectHook();
    return nullptr;
}

void MainWindow::connectHook()
{
    // menu bar
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
