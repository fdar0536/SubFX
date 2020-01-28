#ifndef ASSINITBG_HPP
#define ASSINITBG_HPP

#include "QRunnable"
#include "QObject"

#include "../common/subfxassinit.hpp"

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

protected:

    explicit AssInitBg(QObject *parent = nullptr) :
        QObject(parent),
        QRunnable()
    {}

private:

    AssInitBg() {}

    std::string m_jsonFileName;

    std::shared_ptr<SubFXAssInit> m_assConfig;

};

#endif // ASSINITBG_HPP
