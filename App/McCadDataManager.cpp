#include "PreCompiled.h"
#include "McCadDataManager.h"

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include <App/Application.h>
#include <qmessagebox.h>

McCadDataManager::McCadDataManager():_document(0)
{
    this->connectApplicationCreatedDocument = App::GetApplication().signalNewDocument.connect(boost::bind
        (&McCadDataManager::slotCreatedDocument, this, _1));

    this->connectApplicationDeletedDocument = App::GetApplication().signalDeleteDocument.connect(boost::bind
        (&McCadDataManager::slotDeletedDocument, this, _1));
}


McCadDataManager::McCadDataManager(App::Document *doc): _document(doc)
{
    this->connectApplicationCreatedDocument = App::GetApplication().signalNewDocument.connect(boost::bind
        (&McCadDataManager::slotCreatedDocument, this, _1));

    this->connectApplicationDeletedDocument = App::GetApplication().signalDeleteDocument.connect(boost::bind
        (&McCadDataManager::slotDeletedDocument, this, _1));

    MaterialManager * pMat = new MaterialManager();
    string name = doc->getName();
    insertMatManager(name,pMat);

    McCadData *pData = new McCadData();
    insertData(name,pData);
}


McCadDataManager::~McCadDataManager()
{
    this->connectApplicationCreatedDocument.disconnect();
    this->connectApplicationDeletedDocument.disconnect();
}


void McCadDataManager::slotCreatedDocument(const App::Document& Doc)
{    
    //QMessageBox::warning(NULL,QLatin1String("Warning"),QLatin1String("Please set the working directory firstly!"));

    MaterialManager * pMat = new MaterialManager();
    string name = Doc.getName();
    insertMatManager(name,pMat);

    McCadData *pData = new McCadData();
    insertData(name,pData);
}


void McCadDataManager::slotDeletedDocument(const App::Document& Doc)
{   
    string name = Doc.getName();
    deleteMatManager(name);
    deleteData(name);   
}

bool McCadDataManager::insertData(string & strDoc, McCadData *& data )
{
    pair<map<string,McCadData*>::iterator,bool> result;
    result = m_mapDocData.insert(pair<string,McCadData *>(strDoc,data));

    if(result.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool McCadDataManager::insertMatManager(string & strDoc, MaterialManager *& man )
{
    pair<map<string,MaterialManager*>::iterator,bool> result;
    result = m_mapDocMat.insert(pair<string,MaterialManager *>(strDoc,man));

    if(result.second)
    {
        return true;
    }
    else
    {
        return false;
    }
}

MaterialManager * McCadDataManager::getMatManager(string & strDoc)
{
    map<string,MaterialManager*>::iterator it;
    it = m_mapDocMat.find(strDoc);
    if(it == m_mapDocMat.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

McCadData * McCadDataManager::getData(string & strDoc)
{
    map<string,McCadData*>::iterator it;
    it = m_mapDocData.find(strDoc);
    if(it == m_mapDocData.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

void McCadDataManager::deleteData(string & strDoc)
{
    map<string,McCadData*>::iterator it;
    it = m_mapDocData.find(strDoc);
    if(it != m_mapDocData.end())
    {        
        m_mapDocData.erase(it);
        return;
    }
    else
    {
        return;
    }
}


void McCadDataManager::deleteMatManager(string & strDoc)
{
    map<string,MaterialManager*>::iterator it;
    it = m_mapDocMat.find(strDoc);
    if(it != m_mapDocMat.end())
    {
        m_mapDocMat.erase(it);
        return;
    }
    else
    {
        return;
    }
}


void McCadDataManager::TurnOnConvertor()
{
    if (!m_ConvertorSwitch)
        m_ConvertorSwitch = true;
}



bool McCadDataManager::IsConvertorOn()
{
    return m_ConvertorSwitch;
}
