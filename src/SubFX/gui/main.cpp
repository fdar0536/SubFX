#include <iostream>

#include "pybind11/embed.h"
#undef B0
#include "QApplication"

#include "ui/mainwindow.hpp"

namespace py = pybind11;

int main(int argc, char **argv)
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)
    std::cerr << "Sorry, this program is not ready." << std::endl;
    return 0;
    /*
    QApplication a(argc, argv);
    py::scoped_interpreter guard{};

    MainWindow *w(MainWindow::create(nullptr));
    if (!w)
    {
        std::cerr << "Fail to allocate memory." << std::endl;
        return 1;
    }

    w->show();
    int ret = a.exec();
    delete w;
    return ret;
    */
}
