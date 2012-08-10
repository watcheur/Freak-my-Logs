/****************************************************************************
 **
 ** Copyright (C) 2012 Mathieu Moriceau
 ** All rights reserved.
 ** Contact: Watcheur (watcheur@orange.fr)
 **
 ****************************************************************************/

#ifndef LOGS_H
#define LOGS_H

#include "window.h"
#include <QMainWindow>

#define UPPERCASE(c) (('a' <= (c) && (c) <= 'z') ? ((x - 'a') + 'A') : (c))
#define MAX_ID  10

// UnitFlag bitfield
// See: http://www.wowpedia.org/UnitFlag

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

class Logs : public QDialog
{
    Q_OBJECT

public:
    explicit Logs(std::string filename, QWidget *parent = 0);
    ~Logs();

    // Interface
    void createFilesTable();

    // Parser
    std::deque<std::string> parse_log(std::string line);
    std::string get_args(const char *content, int *pos);
    void parse_spells_id(std::string line);
    void parse_spells_name(std::string line);
    void parse_boss_name(std::string line);

    void track_spell(std::deque<std::string>);
    void track_boss(std::deque<std::string>);
	
	t_events add_event(int, int, std::string, std::string, std::string, int);

private slots:
    void launch_parse();

private:
    // Interface
    std::string     filename;
    QPushButton     *createButton(const QString &text, const char *member);
    QGridLayout     *parseLayout;
    QPushButton     *parseButton;
    QCheckBox       *debugbox;

    QLabel          *bossLabel;
    QLineEdit       *trackedBoss;

    QLabel          *spellsLabel;
    QLineEdit       *trackedSpells;

    // Parser
    std::ifstream   filedes;
    std::string actual_boss;
    bool on_boss;
    //std::deque<std::string> logs;
	std::list<t_events>	logs;

    // Players
    std::deque<std::string> players;

    // Boss
    std::deque<std::string> boss_name;

    // Spells
    std::deque<int> spells_id;
};

#endif // LOGS_H
