#ifndef MCCADCONVERSION_H
#define MCCADCONVERSION_H

#include <QThread>
#include <TCollection_AsciiString.hxx>

#include <iostream>

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

using namespace std;

class McCadConversion : public QThread
{

public:
    McCadConversion();
    ~McCadConversion();
    void Setting(const QString strFile,
                 const QString strConfig,
                 const QString strMat,
                 const QString strOpt);                // Set the names of files
    double GetRunTime();

    TCollection_AsciiString getCodeOpt();              // option of MC codes, tripoli or mcnp


    void run();   
    void stop();

private:

    TCollection_AsciiString m_strStepFile;              // The converted model file
    TCollection_AsciiString m_strMatFile;               // The material file
    TCollection_AsciiString m_strConfigFile;            // The config file
    TCollection_AsciiString m_strCodeOption;            // option of MC codes, tripoli or mcnp

    volatile bool stopped;

};




#endif // MCCADCONVERSION_H
