#include "logpanel.hpp"
#include "ui_logpanel.h"

std::pair<LogPanel *, const char *>
LogPanel::create(QWidget *parent)
{
    LogPanel *ret(new (std::nothrow) LogPanel(parent));
    if (!ret)
    {
        return std::make_pair(nullptr,
                              "Fail to allocate memory");
    }

    Ui::LogPanel *ui(new (std::nothrow) Ui::LogPanel());
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

LogPanel::~LogPanel()
{
    delete m_ui;
}
