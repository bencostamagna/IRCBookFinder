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

#define WINDOW_WIDTH 	800
#define WINDOW_HEIGHT	600

#define WINDOW_MARGIN	30
#define LINE_HEIGHT 	50

class MainWindow : public QWidget
{
    Q_OBJECT
    public:
	explicit MainWindow(QWidget *parent = 0);
	virtual ~ MainWindow();
    private slots:
	void OnSearchButton();
    private:
	    QPushButton 	*m_buttonSearch;
	    QTextEdit 		*m_textSearch;
};

#endif /* !MAINWINDOW_H */
