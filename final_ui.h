/*
 *  final_ui.h
 *  Freak My Logs
 *
 *  Created by Mathieu MORICEAU on 09/08/12.
 *  Copyright 2012. All rights reserved.
 *
 */

#ifndef	IFINAL_H
#define IFINAL_H

#include <QPushButton>
#include "window.h"
#include "logs.h"

class IFinal : public QDialog {
	Q_OBJECT
	
public:
	IFinal(std::list<t_events>, std::deque<int>, std::deque<std::string>, std::deque<std::string>, QWidget *parent = 0);
	~IFinal();

private slots:
	void launch_interface();
	
private:
	std::list<t_events>	logs;
	std::deque<int> spells;
	std::deque<std::string> boss;
	std::deque<std::string> players;
	QPushButton	*createButton(const QString &text, const char *member);
	
	QComboBox	*boss_list;
	QTextEdit	*textarea;
};

std::ostream& operator<<(std::ostream& os, t_events const &l);

#endif /* IFINAL_H */