#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <utility>

#include "QString"
#include "QMainWindow"
#include "QCloseEvent"

#include "mainpanel.hpp"
#include "logpanel.hpp"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static std::pair<MainWindow *, const char *>
    create(QWidget *parent = nullptr);

    ~MainWindow() override;

protected:

    explicit MainWindow(QWidget *parent = nullptr) :
        QMainWindow(parent),
        m_ui(nullptr),
        m_title(""),
        m_mainPanel(nullptr),
        m_logPanel(nullptr)
    {}

private slots:

    // for menu bar
    void onOpenTriggled(bool);

    void onAboutQtTriggled(bool);

    void onExitTriggled(bool);

private:

    MainWindow() {}

    Ui::MainWindow *m_ui;

    QString m_title;

    MainPanel *m_mainPanel;

    LogPanel *m_logPanel;

    const char *init();

    void connectHook();

    void closeEvent(QCloseEvent *) override;

    void handleExit();
};

#endif // MAINWINDOW_HPP
