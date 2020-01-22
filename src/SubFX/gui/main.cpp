#include <iostream>

#include "QApplication"

#include "ui/mainwindow.hpp"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    MainWindow *w(nullptr);
    const char *err(nullptr);
    std::tie(w, err) = MainWindow::create(nullptr);
    if (err)
    {
        std::cerr << err << std::endl;
        return 1;
    }

    w->show();
    int ret = a.exec();
    delete w;
    return ret;
}
