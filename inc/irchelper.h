/*
 * irchelper.h
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef IRCHELPER_H
#define IRCHELPER_H

#include "libircclient.h"
#include "libirc_rfcnumeric.h"

#include <QString>
#include <QStringList>
#include <QThread>
#include <map>

#include "filehelper.h"

#define IRC_TIMEOUT 30

class IrcHelper: public QThread
{
    Q_OBJECT
    public:
	static IrcHelper* getInstance();
	~IrcHelper();

	void run();
	bool isConnected();
	void ProtocolMessageBox();
	void setServerData(QString server, int port, QString nick, QString channel);

	void OnConnected();
	void OnFileTransfer(irc_session_t* session, const char* nick, const char* addr, const char* filename, unsigned long size, irc_dcc_t dccid);
	void OnFileRcvd(irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length);
	void OnSearchResults(irc_dcc_t dccid);
	void OnBookDownloadComplete(irc_dcc_t dccid);

    signals:
	void sig_connected(bool);
	void sig_searchResults(QStringList);
	void sig_status(QString status);

    public slots:
	void searchString(QString str);
	void launchDownload(QString str);
	void disconnect();

    private:
	IrcHelper();

    private:
	irc_session_t *m_session;
	QString m_server;
	int m_port;
	QString m_nick;
	QString m_channel;

	std::map<irc_dcc_t, FileInfo> m_fileCatalog;

	bool m_bSearching;
	bool m_bDownloading;
};

#endif /* !IRCHELPER_H */
