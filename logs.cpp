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
    this->trackedBoss = new QLineEdit("Morchok,Ultraxion,Hagara");

    this->spellsLabel = new QLabel(tr("<strong>Spells</strong>:"));
    this->trackedSpells = new QLineEdit(trUtf8("42-TROP FORT,43-TROP NUL"));

    this->parseButton = createButton(tr("&Parse"), SLOT(launch_parse()));

    bossLayout->addWidget(bossLabel);
    bossLayout->addWidget(trackedBoss);

    spellsLayout->addWidget(spellsLabel);
    spellsLayout->addWidget(trackedSpells);

    parseLayout->addWidget(fileinfo, 0, 0);
    parseLayout->addWidget(parseButton, 3, 3);
    parseLayout->addLayout(bossLayout, 1, 0);
    parseLayout->addLayout(spellsLayout, 2, 0);

    secondLayout->addLayout(parseLayout, 0, 0);

    setLayout(secondLayout);
    setWindowTitle(tr("Infos"));
    this->show();
    this->resize(700, 100);
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

    while ((int)line.size() >= pos)
    {
        if (line.c_str()[pos] == '-')
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

void Logs::parse_spells_name(std::string line)
{
    int idx = 0;
    int pos = 0;
    char name[255];

    memset(&name, '\0', 255);
    while ((int)line.size() >= pos)
    {
        if (line.c_str()[pos] == '-')
        {
            pos += 1;
            while (line.c_str()[pos] != ',')
            {
                name[idx] = line.c_str()[pos];
                idx++;
                pos++;
            }
            this->spells_name.push_back(std::string(name));
            memset(&name, '\0', 255);
            idx = 0;
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
    parse_spells_id(this->trackedSpells->text().toStdString());
    parse_spells_name(this->trackedSpells->text().toStdString());
    parse_boss_name(this->trackedBoss->text().toStdString());

    for (int i = 0; (int)this->spells_id.size() > i; ++i)
        std::cout << this->spells_id[i] << " - " << this->spells_name[i] << std::endl;

    for (int j = 0; (int)this->boss_name.size() > j; ++j)
        std::cout << this->boss_name[j] << std::endl;
    return;
    std::string line;
    std::deque<std::string> args;

    this->filedes.open(this->filename.c_str(), std::ifstream::in);
    if (filedes.is_open())
    {
        while (filedes.good())
        {
            getline(filedes, line);
            args = parse_log(line);
            line.clear();
        }
    }
    else
        std::cout << "Unable to open file" << filename << std::endl;
}

void    Logs::track_boss(std::deque<std::string> args)
{

}

std::deque<std::string>    Logs::parse_log(std::string line)
{
    std::string arg;
    std::deque<std::string> ret;
    bool    is_name = false;
    int     count = 0;

    for (int i = 0; i < (int)line.size(); ++i)
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
    return (ret);
}

/* SPELL */
std::string    Logs::track_spell(std::deque<std::string> args)
{
    return("YEAH");
}

void    Logs::potion_resum()
{

}
