#include "assinitbg.hpp"

// public member functions
AssInitBg *AssInitBg::create(QObject *parent)
{
    AssInitBg *ret(new (std::nothrow) AssInitBg(parent));
    if (!ret)
    {
        return nullptr;
    }

    return ret;
}

void AssInitBg::setJsonFileName(std::string &jsonFileName)
{
    m_jsonFileName = jsonFileName;
}

std::shared_ptr<SubFXAssInit> AssInitBg::getAssConfig() const
{
    return m_assConfig;
}

void AssInitBg::run()
{
    emit stateChanged(tr("Parsing config..."));
    const char *err(nullptr);
    std::tie(m_assConfig, err) = SubFXAssInit::create(m_jsonFileName);
    if (err)
    {
        QString tmp(tr(err));
        tmp = tr("Error: ") + tmp;
        emit message(tmp);
        emit stateChanged("");
        return;
    }

    emit stateChanged(tr("Parse Success"));
    emit parseSuccess();
}
