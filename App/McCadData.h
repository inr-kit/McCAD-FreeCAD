#ifndef MCCADDATA_H
#define MCCADDATA_H

#include <QString>
#include <vector>
#include <map>
#include <QThread>

#include "Material/Material.h"
#include "Material/MaterialManager.h"

using namespace std;

class McCadData
{

public:
    McCadData();
    virtual ~McCadData();

    void SetWorkingDir(QString dir);
    QString GetWorkingDir();

private:

    QString m_strWorkingDir;                // file folder for working space

};


#endif // MCCADDATA_H
