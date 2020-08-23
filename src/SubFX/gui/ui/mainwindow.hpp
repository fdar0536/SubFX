#pragma once

#include "QMainWindow"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static MainWindow *create(QWidget *parent = nullptr);

    ~MainWindow();

private:

    explicit MainWindow(QWidget *parent = nullptr) :
        QMainWindow(parent),
        m_ui(nullptr)
    {}

    MainWindow(const MainWindow &) = delete;

    MainWindow() = delete;

    MainWindow &operator=(MainWindow) = delete;

    Ui::MainWindow *m_ui;
};
