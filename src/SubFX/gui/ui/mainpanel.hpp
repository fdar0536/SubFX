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

signals:

    void message(QString &);

public slots:

    void parseConfig(std::string &jsonFileName);

protected:

    explicit MainPanel(QWidget *parent = nullptr) :
        QWidget(parent),
        m_ui(nullptr),
        m_dirty(false)
    {}

private:

    MainPanel() {}

    Ui::MainPanel *m_ui;

    bool m_dirty;

    std::shared_ptr<SubFXAssInit> m_assConfig;

};

#endif // MAINPANEL_HPP
