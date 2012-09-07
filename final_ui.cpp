/*
 *  final_ui.cpp
 *  Freak My Logs
 *
 *  Created by Mathieu MORICEAU on 09/08/12.
 *  Copyright 2012. All rights reserved.
 *
 */

#include "final_ui.h"

IFinal::IFinal(std::list<t_events> logs, std::deque<int> spells, std::deque<std::string> boss, std::deque<std::string> players, QWidget *parent) : QDialog(parent)
{
    QGridLayout *thirdLayout = new QGridLayout;
	QPushButton	*launchButton = createButton(tr("&Changer"), SLOT(launch_interface()));
	
	this->boss_list = new QComboBox;
	this->textarea = new QTextEdit;
	
	this->logs = logs;
	this->spells = spells;
	this->boss = boss;
	this->players = players;
	
	int i = 0;
	for (i = 0; i < (int)this->boss.size(); ++i)
        boss_list->addItem(QString(this->boss[i].c_str()));
	
	thirdLayout->addWidget(this->boss_list, 0, 0);
	thirdLayout->addWidget(launchButton, 0, 1);
	thirdLayout->addWidget(this->textarea, 1, 0);
	
	setLayout(thirdLayout);
    setWindowTitle(tr("Interface final"));
    this->resize(300, 600);
    this->show();
}

IFinal::~IFinal()
{
	
}

std::ostream& operator<<(std::ostream& os, t_events const &l)
{
	std::deque<std::string>	message;
	
	message.push_back(" a subit "); // DAMAGE
	message.push_back(" a gagné "); // BUFF
	message.push_back(" a subit "); // DEBUFF
	message.push_back(" est apparu le "); // SPAWN
	message.push_back(" est mort par "); // DIE
	message.push_back(" one shot par "); // INSTAKILL
	message.push_back(" a été engagé a "); // BOSS_PULL
	message.push_back(" est mort a "); // BOSS_DIE
	message.push_back("Le raid a wipe."); // WIPE
	
	switch (l.event_type) {
		case BOSS_PULL:
		case BOSS_DIE:
		case SPAWN:
			os << l.event_target << message[l.event_type] << l.event_date << std::endl;
			break;
		case BUFF:
		case DEBUFF:
		case INSTAKILL:
			os << l.event_target << message[l.event_type] << l.event_name << "(" << l.event_id << ") a " << l.event_date << std::endl;
			break;
		case DAMAGE:
			os << l.event_target << message[l.event_type] << l.damage << " de dégats par " << l.event_name << "(" << l.event_id << ") a " << l.event_date << std::endl;
			break;
		case WIPE:
			os << message[l.event_type] << std::endl;
		default:
			break;
	}
	return (os);
}

void    IFinal::launch_interface()
{
	std::list<t_events>::iterator it_boss;
	std::list<t_events>	actual_boss;
    
    int spells_idx;
    int players_idx;
    int count;
    
    bool get_name;
	
	/*
	 enum event_list {
	 DAMAGE = 0,
	 BUFF,
	 DEBUFF,
	 SPAWN,
	 DIE,
	 INSTAKILL,
	 BOSS_PULL,
	 BOSS_DIE,
	 WIPE
	 };
	 
	 typedef struct	s_events {
	 int	event_type;
	 int	event_id;
	 int	damage;
	 std::string	event_name;
	 std::string	event_date;
	 std::string	event_target;
	 }				t_events;
	 */
	this->textarea->clear();
	for (it_boss = this->logs.begin(); it_boss != this->logs.end(); ++it_boss)
	{
		if (it_boss->event_target == this->boss_list->currentText().toStdString() && it_boss->event_type == BOSS_PULL)
		{
			actual_boss.push_back(*it_boss);
			it_boss++;
			while (it_boss->event_type != BOSS_DIE)
			{
				actual_boss.push_back(*it_boss);
				it_boss++;
			}
			actual_boss.push_back(*it_boss);
			break;
		}
	}
    
    for (spells_idx = 0; spells_idx < (int)this->spells.size(); ++spells_idx)
    {
        for (players_idx = 0, count = 0; players_idx < (int)this->players.size(); ++players_idx)
        {
            count = 0;
            for (it_boss = actual_boss.begin(); it_boss != actual_boss.end(); ++it_boss)
            {
                if (get_name == false && it_boss->event_id == this->spells[spells_idx])
                {
                    this->textarea->append(QString().fromUtf8(it_boss->event_name.c_str()));
                    get_name = true;
                }
                
                if (it_boss->event_id == this->spells[spells_idx] && it_boss->event_target == this->players[players_idx])
                    count += 1;
            }
            if (count > 0)
                this->textarea->append("  " + QString().fromUtf8(this->players[players_idx].c_str()) + ": x" +
                                       QString::number(count));
        }
        this->textarea->append("");
        get_name = false;
    }
}

QPushButton *IFinal::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
	
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
