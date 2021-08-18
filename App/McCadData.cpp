#include "PreCompiled.h"
#include "McCadData.h"

McCadData::McCadData()
{    
    m_strWorkingDir = QString::fromLatin1("");
}

McCadData::~McCadData()
{  
}

void McCadData::SetWorkingDir(QString dir)
{
    m_strWorkingDir = dir;
}


QString McCadData::GetWorkingDir()
{
    return m_strWorkingDir;
}
