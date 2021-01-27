#include <iostream>

#include "pybind11/embed.h"
#undef B0
#include "QApplication"

#include "ui/mainwindow.hpp"

namespace py = pybind11;

int main(int argc, char **argv)
{
    py::scoped_interpreter guard{};

    QApplication a(argc, argv);

    MainWindow *w(MainWindow::create(nullptr));
    if (!w)
    {
        std::cerr << "Fail to allocate memory." << std::endl;
        return 1;
    }

    w->show();
    const int ret(a.exec());
    delete w;
    return ret;
}
