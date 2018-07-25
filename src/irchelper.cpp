/*
 * irchelper.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "irchelper.h"

#include "filehelper.h"

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



    IrcHelper::getInstance()->OnConnected();
}


void callback_dcc_recv_file (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    //qDebug() << "event_connect: status " << QString::number(status) << ", length: " << QString::number(length);
    if ( status )
    {
	FileHelper::end_write();
	IrcHelper::getInstance()->ProtocolMessageBox();
    }
    else if ( data == 0 )
    {
	// File transfer has been finished
	qDebug() << "File has been received successfully";
	FileHelper::end_write();
    }
    else
    {
	// More file content has been received. Store it in memory, write to disk or something
	printf ("Received %d bytes of data\n", length );
	FileHelper::write_buffer(data, length);
    }
}



void callback_event_dcc_file(irc_session_t* session, const char* nick, const char* addr, const char* filename, unsigned long size, irc_dcc_t dccid)
{
    qDebug() << "file transfer from " << QString(nick) << " of file " << QString(filename) << ", size "<< QString::number(size);
    irc_dcc_accept(session, dccid, 0, callback_dcc_recv_file);

    FileHelper::init_write(filename);
}



//dummy event_numeric
void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
}


void IrcHelper::ProtocolMessageBox()
{
    QMessageBox msgBox;
    msgBox.setText("error "+QString::number(irc_errno(m_session))+": "+irc_strerror(irc_errno(m_session)));
    msgBox.exec();
 
}



void IrcHelper::OnConnected()
{
    if (irc_cmd_join(m_session, m_channel.toUtf8(), 0 ))
	ProtocolMessageBox();
    emit sig_connected();
}


void IrcHelper::searchString(QString str)
{
    if (irc_cmd_msg(m_session, m_channel.toUtf8(), "@search "+str.toUtf8()))
	ProtocolMessageBox();
}

void IrcHelper::run()
{
    // The IRC callbacks structure
    irc_callbacks_t callbacks;

    // Init it
    memset(&callbacks, 0, sizeof(callbacks));

    // Set up the mandatory events
    callbacks.event_connect = m_event_connect;
    callbacks.event_numeric = event_numeric;
    callbacks.event_dcc_send_req = callback_event_dcc_file;
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



