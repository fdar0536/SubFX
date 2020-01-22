#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <utility>

#include "QString"
#include "QMainWindow"
#include "QCloseEvent"

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

    ~MainWindow();

protected:

    explicit MainWindow(QWidget *parent = nullptr) :
        QMainWindow(parent),
        m_ui(nullptr),
        m_title("")
    {}

private slots:

    // for menu bar
    void onAboutQtTriggled(bool);

    void onExitTriggled(bool);

private:

    MainWindow() {}

    Ui::MainWindow *m_ui;

    QString m_title;

    const char *init();

    void connectHook();

    void closeEvent(QCloseEvent *) override;

    void handleExit();
};

#endif // MAINWINDOW_HPP
