#ifndef MCCADSURFACECARD_H
#define MCCADSURFACECARD_H

#include <iostream>
#include <string.h>
#include <TopoDS_Solid.hxx>

#include <TopoDS_Face.hxx>
#include <Inventor/nodes/SoEventCallback.h>
#include <Gui/View3DInventorViewer.h>

#include <App/PropertyStandard.h>

using namespace std;

class McCadSurfaceCard //: public Gui::SelectionObserver
{

public:
    McCadSurfaceCard();
   ~McCadSurfaceCard();

    void startInteractiveCallback();
    void stopInteractiveCallback();
protected:
    Gui::View3DInventorViewer* getViewer() const;
    SoEventCallbackCB *activeCB;

private:
    static void SelectSurfaceCallback(void * ud, SoEventCallback * n);       // select surface and display the geometry
    static string GetSurfEquation(TopoDS_Face &theFace);
    static string cs;
};

#endif // MCCADSURFACECARD_H
