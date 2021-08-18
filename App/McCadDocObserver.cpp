#include "PreCompiled.h"
#include "McCadDocObserver.h"


McCadDocObserver::McCadDocObserver()
{

}

McCadDocObserver::McCadDocObserver(App::Document * document)
    : DocumentObserver(document)
{

}

/**
  * Invalidate cells that depend on this document object.
  *
  */

void McCadDocObserver::slotCreatedDocument(const App::Document& Doc)
{
    App::Document *doc = App::GetApplication().getActiveDocument();
    if (!doc) return;
}

McCadAppData * McCadDocObserver::GetData()
{
   if (m_pData)
       return m_pData;
}

QString McCadDocObserver::GetData(string &str)
{
    return mapDoc[str];
}
