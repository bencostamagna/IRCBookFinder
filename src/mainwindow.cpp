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
    
    m_buttonDownload = new QPushButton("Download", this);
    m_buttonDownload->setGeometry(600, 2*LINE_OFFSET+WINDOW_MARGIN, BUTTON_WIDTH, LINE_HEIGHT);
    connect(m_buttonDownload, SIGNAL(released()), this, SLOT(OnDownload()));
    m_buttonDownload->setEnabled(false);

    m_listWidget = new QListWidget(this);
    m_listWidget->setGeometry(WINDOW_MARGIN, 2*LINE_OFFSET+WINDOW_MARGIN, 500, 400);

    m_worker = IrcHelper::getInstance();
    //m_worker->setServerData("chat.freenode.net", 6667, "chupacabot", "#chupacabratest");
    m_worker->setServerData("eu.undernet.org", 6667, "chupacabot", "#bookz");
    connect(m_worker, SIGNAL(sig_connected(bool)), this, SLOT(OnConnected(bool)));
    connect(m_worker, SIGNAL(sig_searchResults(QStringList)), this, SLOT(OnSearchResults(QStringList)));
    connect(m_listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectionChanged()));
    connect(m_worker, SIGNAL(sig_status(QString)), this, SLOT(setStatus(QString)));
}


MainWindow::~MainWindow()
{
    delete m_buttonSearch;
    delete m_textSearch;

    delete m_buttonConnect;
    delete m_textStatus;

    delete m_buttonDownload;
    delete m_listWidget;

    if (m_worker->isRunning())
    {
	m_worker->terminate();
	for ( int c = 0; c < 20 && m_worker->isRunning(); ++c)
	    QThread::sleep(1);
    }
}

void MainWindow::setStatus(QString status)
{
    m_textStatus->setText(status);
}


void MainWindow::OnConnected(bool is_connected)
{
    m_buttonSearch->setEnabled(is_connected);
    m_buttonConnect->setEnabled(!is_connected);
    setStatus((is_connected)?"Connected":"Disconnected");
}


void MainWindow::OnConnect()
{
    m_buttonConnect->setEnabled(false);
    m_worker->start();
    setStatus("Connecting...");
}

void MainWindow::OnDownload()
{
    
    if (m_listWidget->selectedItems().count() == 0)
    {
	QMessageBox msgBox;
        msgBox.setText("No item selected");
	msgBox.exec();
    }
    else
    {
	//QMessageBox msgBox;
	//msgBox.setText("Downloading"+m_listWidget->selectedItems().first()->text());
	//msgBox.exec();
	m_worker->launchDownload(m_listWidget->selectedItems().first()->text());
    }
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
	m_worker->searchString(pattern);
    }
}

void MainWindow::OnSelectionChanged()
{
    if (m_listWidget->selectedItems().count() == 0)
    {
	qDebug() << "No selection";
	m_buttonDownload->setEnabled(false);
    }
    else
    {
	qDebug() << "Selection changed to" << m_listWidget->selectedItems().first()->text();
	m_buttonDownload->setEnabled(true);
    }
}


