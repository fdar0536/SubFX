#ifndef MAINPANEL_HPP
#define MAINPANEL_HPP

#include <utility>

#include "../background/assinitbg.hpp"
#undef B0

#include "QWidget"

namespace Ui
{
    class MainPanel;
}

class MainPanel : public QWidget
{
    Q_OBJECT

public:

    static MainPanel *create(QWidget *parent = nullptr);

    ~MainPanel();

    void parseConfig(std::string &jsonFileName);

signals:

    void message(QString &);

    void stateChanged(const QString &, int = 0);

protected:

    explicit MainPanel(QWidget *parent = nullptr) :
        QWidget(parent),
        m_ui(nullptr),
        m_dirty(false),
        m_assConfig(nullptr),
        m_assInitBg(nullptr)
    {}

private:

    MainPanel() {}

    Ui::MainPanel *m_ui;

    bool m_dirty;

    std::shared_ptr<SubFXAssInit> m_assConfig;

    AssInitBg *m_assInitBg;

    void connectHook();

};

#endif // MAINPANEL_HPP
