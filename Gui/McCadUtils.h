#ifndef McCad_UTILS_H
#define McCad_UTILS_H

#include "PreCompiled.h"
#ifndef _PreComp_
#endif

#include <Base/Console.h>
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Document.h>
#include <Gui/MainWindow.h>
#include <Gui/FileDialog.h>
#include <Gui/ViewProviderDocumentObject.h>
#include <Gui/ViewProviderDocumentObjectGroup.h>
#include <App/DocumentObjectGroup.h>
#include "../App/McCadDocObserver.h"

namespace McCadUtils{

    /* functions for inspecting the objects in the tree view */
    bool isGroup(App::DocumentObject*);
    bool isAtRootLevel(App::DocumentObject*);
    std::vector<App::DocumentObject*> findSubobjects(App::DocumentObjectGroup*);

    /* functions for coloring objects */
    void setSeed();
    void colorObjects(std::vector<App::DocumentObject*>);
    void colorGroup(App::DocumentObjectGroup*);
    void colorAllGroups();

    /* functions for the solid numbering */
    int findLargestSolidIndex();
    void renameSolids(std::vector<App::DocumentObject*>, int); 

    void importStepFile();

    QString createGroupName(const QString& path);

    void dumpDocumentObjects();

}



#endif
