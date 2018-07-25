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
#include <QThread>
#include <map>

class IrcHelper: public QThread
{
    Q_OBJECT
    public:
	static IrcHelper* getInstance();
	~IrcHelper();

	void run();
	void ProtocolMessageBox();
	void setServerData(QString server, int port, QString nick, QString channel);

	void OnConnected();
	void OnFileRcvd(irc_dcc_t dccid);


    signals:
	void sig_connected();

    public slots:
	void searchString(QString str);

    private:
	IrcHelper();

    private:
	irc_session_t *m_session;
	QString m_server;
	int m_port;
	QString m_nick;
	QString m_channel;
};

#endif /* !IRCHELPER_H */
