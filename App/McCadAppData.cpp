#include "PreCompiled.h"
#include "McCadAppData.h"

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include <App/Application.h>

using namespace std;

McCadAppData::McCadAppData(): _document(0)
{
    //Gui::Application::Instance->signalNewDocument.connect(boost::bind(&McCadAppData::slotCreatedDocument, this, _1));
    //Gui::Application::Instance->signalDeleteDocument.connect(boost::bind(&McCadAppData::slotDeletedDocument, this, _1));

    this->connectApplicationCreatedDocument = App::GetApplication().signalNewDocument.connect(boost::bind
        (&McCadAppData::slotCreatedDocument, this, _1));

    this->connectApplicationDeletedDocument = App::GetApplication().signalDeleteDocument.connect(boost::bind
        (&McCadAppData::slotDeletedDocument, this, _1));
}

McCadAppData::~McCadAppData()
{
    this->connectApplicationCreatedDocument.disconnect();
    this->connectApplicationDeletedDocument.disconnect();
}

void McCadAppData::slotCreatedDocument(const App::Document& Doc)
{
  cout<<"Create new document!!!!!!!!!!!!!"<<endl;
}


void McCadAppData::slotDeletedDocument(const App::Document& Doc)
{
  cout<<"Delete new document!!!!!!!!!!!!!!!!!!!!!"<<endl;
}
