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
    :	m_bConnected{false},
    	m_bSearching{false},
	m_bDownloading{false}
{
    m_session=NULL;
}

IrcHelper::~IrcHelper()
{}

IrcHelper* IrcHelper::getInstance()
{
    static IrcHelper h;
    return &h;
}

void IrcHelper::setServerData(QString server, int port, QString nick, QString channel)
{
    m_chanInfo.server = server;
    m_chanInfo.port = port;
    m_chanInfo.nick = nick;
    m_chanInfo.channel = channel;
}

void IrcHelper::updateUserList(QStringList l)
{
    QStringListIterator it (l);
    while (it.hasNext())
    {
	QString u = it.next();
	if (u[0] == '@' || u[0] == '+')
	    u.remove(0, 1);
	m_chanInfo.users.append(u);
    }
}


void event_connect(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
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



void event_privmsg(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    QString str_params;
    for (uint i =0; i < count; ++i)
    {
	str_params+="|";
	str_params+= params[i];
    }
    str_params+="|";
    qDebug() << "event_privmsg: event " << QString(event) << ", origin: " << QString(origin) << ", params: " << str_params;
}

void event_channel(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    QString str_params;
    for (uint i =0; i < count; ++i)
    {
	str_params+="|";
	str_params+= params[i];
    }
    str_params+="|";
    qDebug() << "event_channel: event " << QString(event) << ", origin: " << QString(origin) << ", params: " << str_params;
}

void callback_dcc_recv_file (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    IrcHelper::getInstance()->OnFileRcvd(session, id, status, ctx, data, length);
}



void callback_event_dcc_file(irc_session_t* session, const char* nick, const char* addr, const char* filename, unsigned long size, irc_dcc_t dccid)
{
    IrcHelper::getInstance()->OnFileTransfer(session, nick, addr, filename, size, dccid);
}


void event_numeric(irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
    if (event == 353)
    {
	IrcHelper::getInstance()->updateUserList(QString(params[count-1]).split(" "));;
    }
    else if (event == 366)
    {
	IrcHelper::getInstance()->OnChannelJoined();
    }
    else
    {
	// unknown event, just print it on the debug channel
	QString str =  "event_numeric: "+QString::number(event);
	for (uint i = 0; i < count; ++i)
	    str += " | "+QString(params[i]);
       qDebug() << str;

    }
}

void IrcHelper::ProtocolMessageBox()
{
    QMessageBox msgBox;
    msgBox.setText("error "+QString::number(irc_errno(m_session))+": "+irc_strerror(irc_errno(m_session)));
    msgBox.exec();
 
}

void IrcHelper::OnFileTransfer(irc_session_t* session, const char* nick, const char* addr, const char* filename, unsigned long size, irc_dcc_t dccid)
{
    qDebug() << "file transfer from " << QString(nick) << " of file " << QString(filename) << ", size "<< QString::number(size);
    irc_dcc_accept(session, dccid, 0, callback_dcc_recv_file);

    FileHelper::init_write(dccid, filename);
    m_fileCatalog[dccid] = FileInfo(QString(filename), size);
}


void IrcHelper::OnFileRcvd(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    if ( status )
    {
	FileHelper::end_write(id);
	//IrcHelper::getInstance()->ProtocolMessageBox();
	
	qDebug() << "Received status " << QString::number(status) << ": " << irc_strerror(status);
	irc_dcc_accept(session, id, 0, callback_dcc_recv_file);
	//m_bSearching = false;
	//m_bDownloading = false;
	//emit sig_status("Connected");
    }
    else if ( data == 0 )
    {
	qDebug() << "File has been received successfully";
	FileHelper::end_write(id);
	if (m_bSearching)
	    OnSearchResults(id);
	else if (m_bDownloading)
	    OnBookDownloadComplete(id);
    }
    else
    {
	m_fileCatalog[id].received += length;
	qDebug() <<"Received " << QString::number(length) << " bytes of data, Progress: " << QString::number(m_fileCatalog[id].received*100 / m_fileCatalog[id].size) << "%";
	FileHelper::write_buffer(id, data, length);
    }
}


void IrcHelper::OnBookDownloadComplete(irc_dcc_t dccid)
{
    QString filename = m_fileCatalog[dccid].filename;
    if (m_fileCatalog.find(dccid) == m_fileCatalog.end())
	return;
    qDebug() << "Download complete to file " << filename;
    m_bDownloading = false;
}


void IrcHelper::OnSearchResults(irc_dcc_t dccid)
{
    QString filename = m_fileCatalog[dccid].filename;
    if (m_fileCatalog.find(dccid) == m_fileCatalog.end())
	return;

    qDebug() << "Search results stored in file " << filename;
    QString content;
    FileHelper::extract_zip(filename, content);

    QStringList list = content.split('\n', QString::SkipEmptyParts);

    QMutableListIterator<QString> i(list);
    while (i.hasNext())
    {
	QString buf = i.next();
	if (buf[0] != '!')
	    i.remove();
    }

    qDebug() << "there are " << list.size() << " results";
    m_bSearching = false;
    emit sig_searchResults(list);
}


void IrcHelper::OnChannelJoined()
{
    QListIterator<QString> i(m_chanInfo.users);
    while (i.hasNext())
	qDebug() << i.next();
    if (m_chanInfo.users.indexOf(QRegExp("SearchOok")) != -1)
    {
	qDebug() << "Search bot online";
	m_bConnected = true;
    }
    else
    {
	qDebug() << "Search bot offline";
	QMessageBox msg;
	msg.setText("Search bot is offline");
	msg.show();
    }
    emit sig_connected(true);
}


void IrcHelper::OnConnected()
{
    if (irc_cmd_join(m_session, m_chanInfo.channel.toUtf8(), 0 ))
	ProtocolMessageBox();
}


void IrcHelper::searchString(QString str)
{
    if (irc_cmd_msg(m_session, m_chanInfo.channel.toUtf8(), "@search "+str.toUtf8()))
	ProtocolMessageBox();
    qDebug() << "Searching " << str;
    m_bSearching=true;
}

void IrcHelper::launchDownload(QString str)
{
    QString msg = str.split("::").first().trimmed();
    qDebug() << "Sending: " << msg;
    if (irc_cmd_msg(m_session, m_chanInfo.channel.toUtf8(), msg.toUtf8()))
	ProtocolMessageBox();
    m_bDownloading=true;
}

void IrcHelper::disconnect()
{
    qDebug() << "disconnect signal received";
    irc_disconnect(m_session);
    emit sig_connected(false);
}

bool IrcHelper::isConnected()
{
    if (!m_session)
	return false;
    return irc_is_connected(m_session) && m_bConnected;
}

void IrcHelper::run()
{
    // The IRC callbacks structure
    irc_callbacks_t callbacks;

    // Init it
    memset(&callbacks, 0, sizeof(callbacks));

    // Set up the mandatory events
    callbacks.event_connect = event_connect;
    callbacks.event_numeric = event_numeric;
    callbacks.event_dcc_send_req = callback_event_dcc_file;
    callbacks.event_privmsg = event_privmsg;
    callbacks.event_channel = event_channel;

    m_chanInfo.users.clear();
    
    // Now create the m_session
    m_session = irc_create_session(&callbacks);

    if (!m_session || irc_connect(m_session, m_chanInfo.server.toUtf8(), m_chanInfo.port, 0, m_chanInfo.nick.toUtf8(), m_chanInfo.nick.toUtf8(), m_chanInfo.nick.toUtf8()))
    {
	emit sig_connected(false);
	ProtocolMessageBox();
	return;
    }

    if (irc_run(m_session))
    {
	ProtocolMessageBox();
	return;
    }
    qDebug() << "disconnected.";
}



