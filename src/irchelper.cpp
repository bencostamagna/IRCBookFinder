/*
 * irchelper.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "irchelper.h"

#include <cstring>
#include <QDebug>
#include <QMessageBox>

	    //("chat.freenode.net", 6667, "chupacabot", "#chupacabratest");
IrcHelper::IrcHelper()
{}

IrcHelper::~IrcHelper()
{}

IrcHelper* IrcHelper::getInstance()
{
    static IrcHelper h;
    return &h;
}

void IrcHelper::setServerData(QString server, int port, QString nick, QString channel)
{
    m_server = server;
    m_port = port;
    m_nick = nick;
    m_channel = channel;
}


void m_event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    QString str_params;
    for (uint i =0; i < count; ++i)
    {
	str_params+="|";
	str_params+= params[i];
    }
    str_params+="|";
    qDebug() << "event_connect: event " << QString(event) << ", origin: " << QString(origin) << ", params: " << str_params;


    if (irc_cmd_join( session, "#chupacabratest", 0 ))
	qDebug() << "join failed";

    IrcHelper::getInstance()->OnConnected();
}

//dummy event_numeric
void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{

    //QString str_params;
    //for (uint i =0; i < count; ++i)
    //{
	//str_params+="|";
	//str_params+= params[i];
    //}
    /*str_params+="|";*/
    //qDebug() << "event_numeric: event " << QString::number(event) << ", origin: " << QString(origin) << ", params: " << str_params;
}


void IrcHelper::ProtocolMessageBox()
{
    QMessageBox msgBox;
    msgBox.setText("error "+QString::number(irc_errno(m_session))+": "+irc_strerror(irc_errno(m_session)));
    msgBox.exec();
 
}

void IrcHelper::OnConnected()
{
    QMessageBox msgBox;
    msgBox.setText("CONNECTED");
    msgBox.exec();
    emit sig_connected();
}

void IrcHelper::run()
{
    // The IRC callbacks structure
    irc_callbacks_t callbacks;

    // Init it
    memset ( &callbacks, 0, sizeof(callbacks) );

    // Set up the mandatory events
    callbacks.event_connect = m_event_connect;
    callbacks.event_numeric = event_numeric;

    // Set up the rest of events

    // Now create the m_session
    m_session = irc_create_session(&callbacks);

    if (!m_session)
    {
	ProtocolMessageBox();
	return;
    }

    // Connect to a regular IRC server
    if (irc_connect(m_session, m_server.toUtf8(), m_port, 0, m_nick.toUtf8(), m_nick.toUtf8(), m_nick.toUtf8()))
    {
	ProtocolMessageBox();
	return;
    }

    if (irc_run(m_session))
    {
	ProtocolMessageBox();
	return;
    }
}



