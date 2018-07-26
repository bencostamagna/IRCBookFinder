/*
 * mainwindow.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "mainwindow.h"

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);


    m_buttonSearch = new QPushButton("Search", this);
    m_buttonSearch->setGeometry(600, LINE_OFFSET+WINDOW_MARGIN, BUTTON_WIDTH, LINE_HEIGHT); 
    connect(m_buttonSearch, SIGNAL (released()), this, SLOT (OnSearchButton()));
    m_buttonSearch->setEnabled(false);

    m_textSearch = new QTextEdit(this);
    m_textSearch->setGeometry(WINDOW_MARGIN, LINE_OFFSET+WINDOW_MARGIN, 500, LINE_HEIGHT);

    m_buttonConnect = new QPushButton("Connect", this);
    m_buttonConnect->setGeometry(600, WINDOW_MARGIN, BUTTON_WIDTH, LINE_HEIGHT); 
    connect(m_buttonConnect, SIGNAL (released()), this, SLOT (OnConnect()));
    
    m_textStatus= new QTextEdit(this);
    m_textStatus->setGeometry(WINDOW_MARGIN, WINDOW_MARGIN, 500, LINE_HEIGHT);
    m_textStatus->setEnabled(false);
    setStatus("Disconnected");
    
    m_listWidget = new QListWidget(this);
    m_listWidget->setGeometry(WINDOW_MARGIN, 2*LINE_OFFSET+WINDOW_MARGIN, 500, 400);

    m_worker = IrcHelper::getInstance();
    //m_worker->setServerData("chat.freenode.net", 6667, "chupacabot", "#chupacabratest");
    m_worker->setServerData("eu.undernet.org", 6667, "chupacabot", "#bookz");
    connect(m_worker, SIGNAL(sig_connected()), this, SLOT(OnConnected()));
    connect(this, SIGNAL(sig_search(QString)), m_worker, SLOT(searchString(QString)));
    connect(m_worker, SIGNAL(sig_searchResults(QStringList)), this, SLOT(OnSearchResults(QStringList)));
}


MainWindow::~MainWindow()
{
    delete m_buttonSearch;
    delete m_textSearch;

    delete m_buttonConnect;
    
    delete m_worker;
}

void MainWindow::setStatus(QString status)
{
    m_textStatus->setText(status);
}


void MainWindow::OnConnected()
{
    m_buttonSearch->setEnabled(true);
    m_buttonConnect->setEnabled(false);
    setStatus("Connected");
}

void MainWindow::OnDisconnected()
{
    m_buttonSearch->setEnabled(false);
    m_buttonConnect->setEnabled(true);
    setStatus("Disconnected");
}

void MainWindow::OnConnect()
{
    m_worker->start();
    setStatus("Connecting...");
}


void MainWindow::OnSearchResults(QStringList results)
{
    setStatus("Connected");
    m_listWidget->clear();
    m_listWidget->addItems(results);
}

void MainWindow::OnSearchButton()
{
    QString pattern = m_textSearch->toPlainText();
    if (pattern.length() == 0)
    {
	QMessageBox msgBox;
        msgBox.setText("Search field is empty");
	msgBox.exec();
    }
    else
    {
	setStatus("Searching...");
	emit sig_search(pattern);
    }
}

