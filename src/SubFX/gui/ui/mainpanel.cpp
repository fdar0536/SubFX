#include <memory>

#include "mainpanel.hpp"
#include "ui_mainpanel.h"

std::pair<MainPanel *, const char *>
MainPanel::create(QWidget *parent)
{
    MainPanel *ret(new (std::nothrow) MainPanel(parent));
    if (!ret)
    {
        return std::make_pair(nullptr,
                              "Fail to allocate memory");
    }

    Ui::MainPanel *ui(new (std::nothrow) Ui::MainPanel());
    if (!ui)
    {
        delete ret;
        return std::make_pair(nullptr,
                              "Fail to allocate memory");
    }

    ret->m_ui = ui;
    ret->m_ui->setupUi(ret);

    return std::make_pair(ret, nullptr);
}

MainPanel::~MainPanel()
{
    delete m_ui;
}
