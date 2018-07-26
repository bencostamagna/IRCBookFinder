/*
 * mainwindow.h
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QStringList>

#include "irchelper.h"

#define WINDOW_WIDTH 	800
#define WINDOW_HEIGHT	600

#define WINDOW_MARGIN	30
#define LINE_HEIGHT 	50
#define LINE_OFFSET	75

#define BUTTON_WIDTH   	150

class MainWindow : public QWidget
{
    Q_OBJECT
    public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~ MainWindow();

    signals:
	void sig_search(QString str);

    private slots:
	void OnSearchButton();
	void OnConnect();

	void OnConnected();
	void OnDisconnected();

	void OnSearchResults(QStringList results);
	void setStatus(QString status);
    private:
	    QPushButton 	*m_buttonSearch;
	    QTextEdit 		*m_textSearch;

	    QPushButton 	*m_buttonConnect;
	    QTextEdit 		*m_textStatus;

	    QListWidget 	*m_listWidget;
	    IrcHelper 		*m_worker;
};

#endif /* !MAINWINDOW_H */
