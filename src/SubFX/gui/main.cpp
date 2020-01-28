#include <iostream>

#include "QApplication"
#include "QMetaType"
#include "pybind11/embed.h"

#include "ui/mainwindow.hpp"

namespace py = pybind11;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    py::scoped_interpreter guard{};

    qRegisterMetaType<QString>("QString&");

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
