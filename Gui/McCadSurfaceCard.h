#ifndef MCCADSURFACECARD_H
#define MCCADSURFACECARD_H

#include <TopoDS_Solid.hxx>
#include <Inventor/nodes/SoEventCallback.h>

class McCadSurfaceCard //: public Gui::SelectionObserver
{
public:
    McCadSurfaceCard();


    //void onSelectionChanged(const Gui::SelectionChanges& msg);
    void selSurface();

    static void SelectSurfaceCallback(void * ud, SoEventCallback * n);       // select surface and display the geometry


};

#endif // MCCADSURFACECARD_H
