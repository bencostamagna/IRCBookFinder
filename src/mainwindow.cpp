/*
 * mainwindow.cpp
 * Copyright (C) 2018 ben <ben@diziet>
 *
 * Distributed under terms of the GPL3 license.
 */

#include "mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    :QWidget(parent)
{
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);


    m_buttonSearch = new QPushButton("Search", this);
    m_buttonSearch->setGeometry(600, WINDOW_MARGIN, 100, LINE_HEIGHT); 
    connect(m_buttonSearch, SIGNAL (released()), this, SLOT (OnSearchButton()));

    m_textSearch = new QTextEdit(this);
    m_textSearch->setGeometry(WINDOW_MARGIN, WINDOW_MARGIN, 500, LINE_HEIGHT);
}


MainWindow::~MainWindow()
{
    delete m_buttonSearch;
    delete m_textSearch;
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
    	QMessageBox msgBox;
        msgBox.setText("Searching for "+pattern.toUtf8()+", not yet implemented");
	msgBox.exec();
 
    }
}

