#ifndef CREATEBOX_H
#define CREATEBOX_H

#include "PreCompiled.h"
#include <TopoDS_Solid.hxx>
#include <Inventor/nodes/SoEventCallback.h>
#include <qstring.h>
#include <QThread>

class CreateBox:public QThread
{
public:
    CreateBox();    
    void MakeBox();
    bool GetSelection();

    void PrintBox();
    void run();

    void SelectSurface();                                                    // add the calleventback of the surface select
    static void SelectSurfaceCallback(void * ud, SoEventCallback * n);       // select surface and display the geometry

    void TestCombiView();

    QString m_ss = QString(QLatin1String("Test"));

};

#endif // CREATEBOX_H
