#ifndef MCCADAPPDATA_H
#define MCCADAPPDATA_H

#include <Base/BaseClass.h>
#include <boost/signals.hpp>

#include <Gui/Document.h>
#include <App/Document.h>

using namespace std;

class AppExport McCadAppData
{

public:
    McCadAppData(void);
    McCadAppData(App::Document *);
    virtual ~McCadAppData();

private:
    /** Checks if a new document was created */
    void slotCreatedDocument(const App::Document& Doc);

    /** Checks if the given document is about to be closed */
    void slotDeletedDocument(const App::Document& Doc);

protected:
    App::Document* getDocument() const;

private:
    App::Document* _document;

    typedef boost::signals::connection Connection;

    Connection connectApplicationCreatedDocument;
    Connection connectApplicationDeletedDocument;



//    void SetWorkingSpace(const QString &strDir);
//    QString GetWorkingSpace();

//    void AddGroup(const QString &strGroup);
//    list<QString> GetGroups();

private:

//    QString m_strWorkingSpace;
//    list<QString> m_listGroups;

//    QString m_MatFile;
//    QString m_ConfigFile;
};


#endif // MCCADAPPDATA_H
