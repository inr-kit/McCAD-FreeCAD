#ifndef MCCADMESSAGER_SIGNAL_H
#define MCCADMESSAGER_SIGNAL_H

#include <QString>

#include <iostream>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>

class McCadMessager_Signal
{
public:
    //! returns static instance
    static McCadMessager_Signal* Instance()
    {
        if( NULL == m_instance )
        {
            m_instance = new McCadMessager_Signal;
        }
        return m_instance;
    }

    //! send message to all registered receiver to update the status
    boost::signals2::signal<void(int)> sig_progress_main;
    boost::signals2::signal<void(int)> sig_progress_part;
    boost::signals2::signal<void(QString)> sig_message_insert;
    boost::signals2::signal<void(QString)> sig_message_append;
    boost::signals2::signal<void(QString)> sig_main_label;
    boost::signals2::signal<void(QString)> sig_part_label;

private:
    McCadMessager_Signal(){}
    static McCadMessager_Signal *m_instance;

};


#endif // MCCADMESSAGER_SIGNAL_H
