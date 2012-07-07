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

Boss::t_boss  tab[] =
{
    {"NULL", "Kohcrom", "Morchok", "Morchok"},
    {"NULL", "NULL", "Seigneur de guerre Zon’ozz", "Warlord Zon'ozz"},
    {"NULL", "NULL", "Yor'sahj l’Insomniaque", "Yor'sahj the Unsleeping"},
    {"NULL", "NULL", "Hagara la Lieuse des tempêtes", "Hagara the Stormbinder"},
    {"NULL", "NULL", "Ultraxion", "Ultraxion"},
    {"NULL", "NULL", "Maître de guerre Corne-Noire", "Warmaster Blackhorn"},
    {"NULL", "Aile de Mort", "Echine d'Aile de Mort", "Spine of Deathwing"},
    {"NULL", "Aile de Mort", "Folie d'Aile de Mort", "Madness of Deathwing"},
};

Potion::t_potion    my_potions[] =
{
    {AGILITY_POTION, "Tol'vir - Agility"},
    {STRENGTH_POTION, "Golemblood - Strength"},
    {INTEL_POTION, "Volcanic - Intel"},
    {REGEN_POTION, "Concentration - Mana"},
    {ARMOR_POTION, "Earthen - Armor"},
    {0, ""},
};

Logs::Logs(std::string filename)
{
    std::string line;
    std::deque<std::string> args;

    this->on_boss = false;
    this->actual_boss = "NO BOSS";
    this->filedes.open(filename.c_str(), std::ifstream::in);
    if (filedes.is_open())
    {
        while (filedes.good())
        {
            getline(filedes, line);
            args = parse_log(line);
            track_boss(args);
            if (args.size() > 12 && args[2] == "SPELL_AURA_APPLIED" && atoi(args[11].c_str()) == 110660)
            {
                this->actual_boss = "Echine d'Aile de Mort";
                this->on_boss = true;
            }
            if (args[2] == "SPELL_AURA_APPLIED")
                this->boss_logs.push_back(track_potions(args));
            line.clear();
        }
    }
    else
        std::cout << "Unable to open file" << filename << std::endl;

    for (int j = 0; j < (int)this->boss_logs.size(); ++j)
    {
        if (this->boss_logs[j] != "")
            std::cout << this->boss_logs[j] << std::endl;
    }
    this->potion_resum();
}

Logs::~Logs()
{
    filedes.close();
}

void    Logs::track_boss(std::deque<std::string> args)
{
    int i = 0;

    for (i = 0; i <= 7; ++i)
    {
        if (args.size() >= 8)
        {
            if (this->on_boss == false && (args[2].find("_DAMAGE") != std::string::npos) && (tab[i].specific_tracking_a == args[8] || tab[i].specific_tracking_b == args[8] || tab[i].name_fr == args[8] || tab[i].name_en == args[8]))
            {
                this->on_boss = true;
                this->actual_boss = tab[i].name_fr;
                this->boss_logs.push_back(tab[i].name_fr + " is engaged.\n");
            }
            else if (this->on_boss == true && (args[2].find("_DIED") != std::string::npos) && (tab[i].specific_tracking_a == args[8] || tab[i].specific_tracking_b == args[8] || tab[i].name_fr == args[8] || tab[i].name_en == args[8]))
            {
                this->on_boss = false;
                this->actual_boss = "NO BOSS";
                this->boss_logs.push_back(tab[i].name_fr + " died.\n");
            }
        }
    }
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

/* POTION */
std::string    Logs::track_potions(std::deque<std::string> args)
{
    std::stringstream ret;
    int idx = 0;

    ret << "";
    while (idx < 5)
    {
        if (my_potions[idx].item_id == atoi(args[11].c_str()))
        {
            if (this->on_boss == true)
                ret << args[4] << " a utilisé une potion " << my_potions[idx].type << " sur le boss " << this->actual_boss << ".";
            else
                ret << args[4] << " a prepote " << my_potions[idx].type << ".";

            if (my_potions[idx].item_id == my_potions[Potion::AGILITY].item_id)
                this->agility.push_back(args[4]);
            else if (my_potions[idx].item_id == my_potions[Potion::STRENGTH].item_id)
                this->strength.push_back(args[4]);
            else if (my_potions[idx].item_id == my_potions[Potion::INTEL].item_id)
                this->intelligence.push_back(args[4]);
            else if (my_potions[idx].item_id == my_potions[Potion::MANA].item_id)
                this->mana.push_back(args[4]);
            else if (my_potions[idx].item_id == my_potions[Potion::ARMOR].item_id)
                this->armor.push_back(args[4]);
            if (count(this->players.begin(), this->players.end(), args[4]) == 0)
                this->players.push_back(args[4]);
        }
        idx++;
    }
    return (ret.str());
}

void    Logs::potion_resum()
{
    int i = 0;

    // Agi resum
    std::cout << "AGILITY" << std::endl;
    for (i = 0; i < (int)this->players.size(); ++i)
    {
        if (count(this->agility.begin(), this->agility.end(), this->players[i]) > 0)
            std::cout << this->players[i] << ": " << count(this->agility.begin(), this->agility.end(), this->players[i]) << std::endl;
    }
    std::cout << std::endl;

    // Str resum
    std::cout << "STRENGTH" << std::endl;
    for (i = 0; i < (int)this->players.size(); ++i)
    {
        if (count(this->strength.begin(), this->strength.end(), this->players[i]) > 0)
            std::cout << this->players[i] << ": " << count(this->strength.begin(), this->strength.end(), this->players[i]) << std::endl;
    }
    std::cout << std::endl;

    // Intel resum
    std::cout << "INTELLIGENCE" << std::endl;
    for (i = 0; i < (int)this->players.size(); ++i)
    {
        if (count(this->intelligence.begin(), this->intelligence.end(), this->players[i]) > 0)
            std::cout << this->players[i] << ": " << count(this->intelligence.begin(), this->intelligence.end(), this->players[i]) << std::endl;
    }
    std::cout << std::endl;

    // Mana resum
    std::cout << "MANA" << std::endl;
    {
        for (i = 0; i < (int)this->players.size(); ++i)
        {
            if (count(this->mana.begin(), this->mana.end(), this->players[i]) > 0)
                std::cout << this->players[i] << ": " << count(this->mana.begin(), this->mana.end(), this->players[i]) << std::endl;
        }
    }
    std::cout << std::endl;

    // Amor resum
    std::cout << "ARMOR" << std::endl;
    {
        for (i = 0; i < (int)this->players.size(); ++i)
        {
            if (count(this->armor.begin(), this->armor.end(), this->players[i]) > 0)
                std::cout << this->players[i] << ": " << count(this->armor.begin(), this->armor.end(), this->players[i]) << std::endl;
        }
    }
    std::cout << std::endl;
}
