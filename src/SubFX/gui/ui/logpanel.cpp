#include "QDateTime"
#include "QFileDialog"
#include "QMessageBox"

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

// public member function
void LogPanel::setLastSavePath(QString &path)
{
    m_lastSavePath = path;
}

QString LogPanel::getLastSavePath() const
{
    return m_lastSavePath;
}

// public slots
void LogPanel::addLog(QString &msg)
{
    QString tmp(getDateTime() + msg);
    m_ui->logList->addItem(tmp);
    m_ui->saveBtn->setEnabled(true);
    m_ui->cleanBtn->setEnabled(true);
}

// private slots
void LogPanel::onSaveBtnClicked(bool)
{
    QString savePath = QFileDialog::getSaveFileName(this,
                                                     "Save log",
                                                     m_lastSavePath,
                                                     "log(*.log)");
    if (savePath == "")
    {
        return;
    }

    FILE *logFile(nullptr);
    logFile = fopen(savePath.toStdString().c_str(), "w");
    if (!logFile)
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Fail to open file."));
        return;
    }

    int index(0);
    for (index = 0; index < m_ui->logList->count(); ++index)
    {
        fputs((m_ui->logList->item(index)->text().toStdString() + "\n").c_str(),
              logFile);
    }

    fclose(logFile);
    m_lastSavePath = savePath;
    QString msg(tr("The log is saved successfully."));
    emit stateChanged(msg);
}

void LogPanel::onCleanBtnClicked(bool)
{
    m_ui->logList->clear();
    m_ui->saveBtn->setEnabled(false);
    m_ui->cleanBtn->setEnabled(false);
}

// private member function
QString LogPanel::getDateTime()
{
    QDateTime time(QDateTime::currentDateTime());
    return (time.toString("yyyy/MM/dd AP hh:mm:ss:zzz") + " ");
}
