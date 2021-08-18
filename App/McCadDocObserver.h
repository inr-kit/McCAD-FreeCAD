#ifndef MCCADDOCOBSERVER_H
#define MCCADDOCOBSERVER_H

#include <App/DocumentObserver.h>
#include "McCadAppData.h"
#include <map>

using namespace std;

class McCadDocObserver : public App::DocumentObserver
{
public:

    McCadDocObserver();
    McCadDocObserver(App::Document* document);
    ~McCadDocObserver() { }
    virtual void slotCreatedDocument(const App::Document& Doc);
    App::Document* getDocument() const { return App::DocumentObserver::getDocument(); }

    McCadAppData * GetData();
    QString GetData(string &str);

private:

    McCadAppData *m_pData;
    map<std::string,QString> mapDoc;
};


#endif // MCCADDOCOBSERVER_H
