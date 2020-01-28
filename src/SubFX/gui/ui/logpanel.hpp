#ifndef LOGPANEL_HPP
#define LOGPANEL_HPP

#include <utility>

#include "QWidget"

namespace Ui
{
    class LogPanel;
}

class LogPanel : public QWidget
{
    Q_OBJECT

public:

    static LogPanel *create(QWidget *parent = nullptr);

    ~LogPanel();

signals:

    void stateChanged(QString &);

public slots:

    void addLog(QString &);

protected:

    explicit LogPanel(QWidget *parent = nullptr) :
        QWidget(parent),
        m_ui(nullptr)
    {}

private slots:

    void onSaveBtnClicked(bool);

private:

    LogPanel() {}

    Ui::LogPanel *m_ui;

    QString getDateTime();
};

#endif // LOGPANEL_HPP
