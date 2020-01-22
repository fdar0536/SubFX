#ifndef MAINPANEL_HPP
#define MAINPANEL_HPP

#include <utility>

#include "QWidget"

namespace Ui
{
    class MainPanel;
}

class MainPanel : public QWidget
{
    Q_OBJECT

public:

    static std::pair<MainPanel *, const char *>
    create(QWidget *parent = nullptr);

    ~MainPanel();

protected:

    explicit MainPanel(QWidget *parent = nullptr) :
        QWidget(parent),
        m_ui(nullptr)
    {}

private:

    MainPanel() {}

    Ui::MainPanel *m_ui;
};

#endif // MAINPANEL_HPP
