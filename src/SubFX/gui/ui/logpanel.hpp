#ifndef LOGPANEL_HPP
#define LOGPANEL_HPP

#include <utility>

#include "QWidget"

namespace Ui {
class LogPanel;
}

class LogPanel : public QWidget
{
    Q_OBJECT

public:

    static std::pair<LogPanel *, const char *>
    create(QWidget *parent = nullptr);

    ~LogPanel();

public slots:

    void addLog(QString &);

protected:

    explicit LogPanel(QWidget *parent = nullptr) :
        QWidget(parent),
        m_ui(nullptr)
    {}

private:

    LogPanel() {}

    Ui::LogPanel *m_ui;
};

#endif // LOGPANEL_HPP
