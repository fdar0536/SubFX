#include "QDateTime"

#include "logpanel.hpp"
#include "ui_logpanel.h"

LogPanel *LogPanel::create(QWidget *parent)
{
    LogPanel *ret(new (std::nothrow) LogPanel(parent));
    if (!ret)
    {
        return nullptr;
    }

    Ui::LogPanel *ui(new (std::nothrow) Ui::LogPanel());
    if (!ui)
    {
        delete ret;
        return nullptr;
    }

    ret->m_ui = ui;
    ret->m_ui->setupUi(ret);

    return ret;
}

LogPanel::~LogPanel()
{
    if (m_ui)
    {
        delete m_ui;
    }
}

// public slots
void LogPanel::addLog(QString &msg)
{
    QString tmp(getDateTime() + msg);
    m_ui->logList->addItem(tmp);
}

// private member function
QString LogPanel::getDateTime()
{
    QDateTime time(QDateTime::currentDateTime());
    return (time.toString("yyyy/MM/dd AP hh:mm:ss:zzz") + " ");
}
