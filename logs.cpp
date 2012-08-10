/****************************************************************************
 **
 ** Copyright (C) 2012 Mathieu Moriceau
 ** All rights reserved.
 ** Contact: Watcheur (watcheur@orange.fr)
 **
 ****************************************************************************/

#include <QtGui>
#include "window.h"
#include "logs.h"
#include "final_ui.h"

// 12/28 21:02:23.629  SPELL_AURA_APPLIED,0x0680000002F548FA,"Rhogar",0x511,0x0,0x0680000002F548FA,"Rhogar",0x511,0x0,79633,"Agilit des Tol'vir",0x10,BUFF

// 79633 - Tol'vir - Agility
// 79634 - Golemblood - Strength
// 79476 - Volcanic - Intel
// 78993 - Concentration - Mana
// 79475 - Earthen - Armor

/*****************************
           INTERFACE
*****************************/

Logs::Logs(std::string filename, QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *bossLayout  = new QHBoxLayout;
    QHBoxLayout *spellsLayout = new QHBoxLayout;
    QGridLayout *secondLayout = new QGridLayout;

    QLabel  *fileinfo = new QLabel(tr("<strong>Logs</strong>: ") + filename.c_str());
    this->filename = filename;
    parseLayout = new QGridLayout;

    this->bossLabel = new QLabel(tr("<strong>Boss</strong>:"));
    this->trackedBoss = new QLineEdit("");
    this->spellsLabel = new QLabel(tr("<strong>Spells</strong>:"));
    this->trackedSpells = new QLineEdit(trUtf8(""));

    this->debugbox = new QCheckBox(tr("Debug mode"));

    this->parseButton = createButton(tr("&Parse"), SLOT(launch_parse()));

    bossLayout->addWidget(bossLabel);
    bossLayout->addWidget(trackedBoss);

    spellsLayout->addWidget(spellsLabel);
    spellsLayout->addWidget(trackedSpells);

    parseLayout->addWidget(fileinfo, 3, 0);
    parseLayout->addWidget(parseButton, 3, 3);
    parseLayout->addLayout(bossLayout, 1, 0);
    parseLayout->addLayout(spellsLayout, 2, 0);

    secondLayout->addLayout(parseLayout, 0, 0);
    secondLayout->addWidget(debugbox, 1, 0);

    setLayout(secondLayout);
    setWindowTitle(tr("Infos"));
    this->show();
}

Logs::~Logs()
{
    filedes.close();
}

QPushButton *Logs::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

/*****************************
            PARSER
*****************************/

void Logs::parse_spells_id(std::string line)
{
    int idx = 0;
    int pos = 0;
    char id[MAX_ID];

    if (line == "")
        return;
    while ((int)line.size() >= pos)
    {
        if (line.c_str()[pos] == ',' || line.c_str()[pos] == 0)
        {
            this->spells_id.push_back(atoi(id));
            memset(&id, '\0', MAX_ID);
            idx = 0;
        }
        if (line.c_str()[pos] >= '0' && line.c_str()[pos] <= '9')
        {
            id[idx] = line.c_str()[pos];
            idx++;
        }
        pos++;
    }
}

void Logs::parse_boss_name(std::string line)
{
    int idx = 0;
    int pos = 0;
    char name[255];

    memset(&name, '\0', 255);
    if (line == "")
        return;
    while ((int)line.size() >= pos)
    {
        if (line.c_str()[pos] == ',' || line.c_str()[pos] == 0)
        {
            this->boss_name.push_back(std::string(name));
            memset(&name, '\0', 255);
            idx = 0;
            pos += 1;
        }
        name[idx] = line.c_str()[pos];
        pos++;
        idx++;
    }
}

void    Logs::launch_parse()
{
    std::string line;
    std::deque<std::string> args;

    this->logs.clear();
    this->spells_id.clear();
    this->boss_name.clear();

    this->on_boss = false;
    parse_spells_id(this->trackedSpells->text().toStdString());
    parse_boss_name(this->trackedBoss->text().toStdString());

    this->filedes.open(this->filename.c_str(), std::ifstream::in);
    if (filedes.is_open())
    {
        std::cout << "Parsing progress..." << std::endl;
        while (filedes.good())
        {
            getline(filedes, line);
            args = parse_log(line);
            track_boss(args);
            track_spell(args);
            line.clear();
        }
    }
    else
        std::cout << "Unable to open file" << this->filename << std::endl;

    std::cout << "Parsing Done..." << std::endl;
    std::cout << "Load new user interface..." << std::endl;
	new IFinal(this->logs, this->spells_id, this->boss_name, this->players);
	this->close();
}

std::deque<std::string>    Logs::parse_log(std::string line)
{
    std::string arg;
    std::deque<std::string> ret;
    bool    is_name = false;
    int     count = 0;

    for (int i = 0; i <= (int)line.size(); ++i)
    {
        if (line.c_str()[i] == '\"' && is_name == false)
        {
            is_name = true;
            i++;
        }
        if (line.c_str()[i] == ',' || (is_name == false && line.c_str()[i] == ' '))
        {
            if (line.c_str()[i] == '\"' && is_name == true)
                is_name = false;
            count++;
            if (line.c_str()[i + 1] != '\"')
                i++;
            if (arg.c_str()[0] == '\"' || arg.c_str()[0] == ' ' || arg.c_str()[0] == ',')
                arg.erase(0, 1);
            if (arg.c_str()[arg.size() - 1] == '\"')
                arg.erase(arg.size() - 1, arg.size());
            ret.push_back(arg);
            arg.clear();
            is_name = false;
        }
        arg.insert(arg.size(), 1, line.c_str()[i]);
    }
    ret.push_back(arg);
    return (ret);
}

t_events	Logs::add_event(int type, int id, std::string name, std::string date, std::string target, int damage)
{
	t_events	ret;
	
	ret.event_type	= type;
	ret.event_id	= id;
	ret.event_name	= name;
	ret.event_date	= date;
	ret.event_target = target;
	ret.damage	= damage;
	return (ret);
}

/* BOSS */

void    Logs::track_boss(std::deque<std::string> args)
{
    if (args.size() > 8)
    {
        if (this->on_boss == false && (args[2].find("_DAMAGE") != std::string::npos) && count(this->boss_name.begin(), this->boss_name.end(), args[4]) >= 1)
        {
            this->actual_boss = args[4];
            this->on_boss = true;
            if (debugbox->isChecked() == true)
                std::cout << args[4] + " engaged " + args[0] + " at "+ args[1] << std::endl;
			this->logs.push_back(this->add_event(BOSS_PULL, -1, args[4], args[1], args[4], -1));
        }

        if (this->on_boss == true && (args[2].find("_DIED") != std::string::npos) && count(this->boss_name.begin(), this->boss_name.end(), args[8]) >= 1)
        {
            this->actual_boss = "";
            this->on_boss = false;
            if (debugbox->isChecked() == true)
                std::cout << args[8] + " died " + args[0] + " at " + args[1]<< std::endl << std::endl;
            this->logs.push_back(this->add_event(BOSS_DIE, -1, args[8], args[1], args[8], -1));
        }
    }
}

/* SPELL */
static int  buff_or_debuff(std::string arg)
{
    if (arg.find("BUFF", 0, 4) != std::string::npos)
        return (BUFF);
    else
        return (DEBUFF);
}

void    Logs::track_spell(std::deque<std::string> args)
{
    if (args.size() >= 12)
    {
        if (args[2].find("_DAMAGE") != std::string::npos)
        {
            if (count(this->spells_id.begin(), this->spells_id.end(), atoi(args[11].c_str())) >= 1)
            {
                if (debugbox->isChecked() == true)
                    std::cout << args[8] + " take " + args[14] + " damage from " + args[12] + " (" + args[11] + ")" << std::endl;
				this->logs.push_back(this->add_event(DAMAGE, atoi(args[11].c_str()), args[12], args[1], args[8], atoi(args[14].c_str())));
            }
        }

        if (args[2].find("_AURA_APPLIED") != std::string::npos)
        {
            if (count(this->spells_id.begin(), this->spells_id.end(), atoi(args[11].c_str())) >= 1)
            {
                if (debugbox->isChecked() == true)
                    std::cout << args[8] << buff_or_debuff(args[14]) << "from " << args[12] << " (" << args[11] << ")" << std::endl;
				this->logs.push_back(this->add_event(buff_or_debuff(args[14]), atoi(args[11].c_str()), args[12], args[1], args[8], -1));
            }
        }

        if (args[2].find("_INSTAKILL") != std::string::npos)
        {
            if (count(this->spells_id.begin(), this->spells_id.end(), atoi(args[11].c_str())) >= 1)
            {
                if (debugbox->isChecked() == true)
                    std::cout << args[8] + " one shot from " + args[12] + " (" + args[11] + ")" << std::endl;
				this->logs.push_back(this->add_event(INSTAKILL, atoi(args[11].c_str()), args[12], args[1], args[8], -1));
            }
        }
		if (count(this->players.begin(), this->players.end(), args[8]) == 0)
			this->players.push_back(args[8]);
    }
}						
