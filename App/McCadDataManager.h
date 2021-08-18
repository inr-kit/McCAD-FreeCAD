#ifndef MCCADDATAMANAGER_H
#define MCCADDATAMANAGER_H

#include <Base/BaseClass.h>
#include <boost/signals.hpp>

#include <Gui/Document.h>
#include <App/Document.h>
#include <App/DocumentObserver.h>

#include "McCadData.h"
#include "../App/Material/MaterialManager.h"

using namespace std;


class McCadAppExport McCadDataManager
{

public:
    McCadDataManager(void);
    McCadDataManager(App::Document *);
    virtual ~McCadDataManager();

    bool insertData(string &, McCadData *& );
    bool insertMatManager(string &, MaterialManager *& );
    McCadData * getData(string &);
    MaterialManager * getMatManager(string & strDoc);

    void TurnOnConvertor();
    bool IsConvertorOn();

private:
    void deleteData(string &);
    void deleteMatManager(string &);

    /** Checks if a new document was created */
    void slotCreatedDocument(const App::Document& Doc);
    /** Checks if the given document is about to be closed */
    void slotDeletedDocument(const App::Document& Doc);

private:

    App::Document* _document;

    typedef boost::signals::connection Connection;

    Connection connectApplicationCreatedDocument;
    Connection connectApplicationDeletedDocument;

    map<string,McCadData *> m_mapDocData;    // the map records the doc and Data    
    map<string,MaterialManager *> m_mapDocMat;


    bool m_ConvertorSwitch = false; // The switch of Convertor

};

#endif // MCCADDATAMANAGER_H
