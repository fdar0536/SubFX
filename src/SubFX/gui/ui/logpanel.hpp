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

    void setLastSavePath(QString &);

    QString getLastSavePath() const;

signals:

    void stateChanged(const QString &, int = 0);

public slots:

    void addLog(QString &);

protected:

    explicit LogPanel(QWidget *parent = nullptr) :
        QWidget(parent),
        m_ui(nullptr),
        m_lastSavePath("")
    {}

private slots:

    void onSaveBtnClicked(bool);

    void onCleanBtnClicked(bool);

private:

    LogPanel() {}

    Ui::LogPanel *m_ui;

    QString m_lastSavePath;

    QString getDateTime();
};

#endif // LOGPANEL_HPP
