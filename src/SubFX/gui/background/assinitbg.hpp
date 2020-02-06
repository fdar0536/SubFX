#ifndef ASSINITBG_HPP
#define ASSINITBG_HPP

#include "../common/subfxassinit.hpp"
#undef B0

#include "QRunnable"
#include "QObject"

class AssInitBg : public QObject, public QRunnable
{

    Q_OBJECT

public:

    static AssInitBg *create(QObject *parent = nullptr);

    void setJsonFileName(std::string &jsonFileName);

    std::shared_ptr<SubFXAssInit> getAssConfig() const;

    void run() override;

signals:

    void message(QString &);

    void stateChanged(const QString &, int = 0);

    void parseSuccess();

protected:

    explicit AssInitBg(QObject *parent = nullptr) :
        QObject(parent),
        QRunnable(),
        m_jsonFileName(""),
        m_assConfig(nullptr)
    {}

private:

    AssInitBg() {}

    std::string m_jsonFileName;

    std::shared_ptr<SubFXAssInit> m_assConfig;

};

#endif // ASSINITBG_HPP
