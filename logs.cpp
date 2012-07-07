#include <QtGui>
#include "window.h"
#include "logs.h"

// 12/28 21:02:23.629  SPELL_AURA_APPLIED,0x0680000002F548FA,"Rhogar",0x511,0x0,0x0680000002F548FA,"Rhogar",0x511,0x0,79633,"Agilit des Tol'vir",0x10,BUFF

// 79633 - Tol'vir - Agility
// 79634 - Golemblood - Strength
// 79476 - Volcanic - Intel
// 78993 - Concentration - Mana
// 79475 - Earthen - Armor

Logs::Logs(std::string filename)
{
    std::string line;
    std::deque<std::string> args;

    this->on_boss = false;
    this->filedes.open(filename.c_str(), std::ifstream::in);
    if (filedes.is_open())
    {
        while (filedes.good())
        {
            getline(filedes, line);
            args = parse_log(line);
            track_boss(args);
            if (this->on_boss == true)
            {
                if (args[2] == "SPELL_AURA_APPLIED")
                    track_potions(args);
            }
        }
    }
    else
        std::cout << "Unable to open file" << std::endl;
}

Logs::~Logs()
{
    filedes.close();
}

void    Logs::track_boss(std::deque<std::string> args)
{
    t_boss  tab[] =
    {
        {"NULL", "NULL", "Morchok", "Morchok"},
        {"NULL", "NULL", "Seigneur de guerre Zon’ozz", "Warlord Zon'ozz"},
        {"NULL", "NULL", "Yor'sahj l’Insomniaque", "Yor'sahj the Unsleeping"},
        {"NULL", "NULL", "Hagara la Lieuse des tempêtes", "Hagara the Stormbinder"},
        {"NULL", "NULL", "Ultraxion", "Ultraxion"},
        {"NULL", "NULL", "Maître de guerre Corne-Noire", "Warmaster Blackhorn"},
        {"Tendons brûlants", "Burning Tendons", "Echinne d'Aile de Mort", "Spine of Deathwing"},
        {"Tentacule d’aile", "Wing Tentacle", "Folie d'Aile de Mort", "Madness of Deathwing"},
    };
    int i;

    for (i = 0; i <= 7; ++i)
    {
        if ((args[2].find("_DAMAGE") == std::string::npos) && (tab[i].specific_tracking_a == args[8] || tab[i].specific_tracking_b == args[8] || tab[i].name_fr == args[8] || tab[i].name_en == args[8]))
        {
            this->on_boss = true;
            this->actual_boss = tab[i].name_fr;
        }
        if ((args[2].find("UNIT_DIED") == std::string::npos) && (tab[i].specific_tracking_a == args[4] || tab[i].specific_tracking_b == args[4] || tab[i].name_fr == args[4] || tab[i].name_en == args[4]))
        {
            this->on_boss = false;
            this->actual_boss = "not on boss";
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
            is_name = true;
        if (line.c_str()[i] == ',' || (is_name == false && line.c_str()[i] == ' '))
        {
            if (line.c_str()[i] == '\"' && is_name == true)
                is_name = false;
            count++;
            i++;
            if (arg.c_str()[0] == '\"' || arg.c_str()[0] == ' ')
            {
                arg.erase(0, 1);
                if (arg.c_str()[arg.size() - 1] == '\"')
                    arg.erase(arg.size() - 1, arg.size());
            }
            ret.push_back(arg);
            arg.clear();
        }
        arg.insert(arg.size(), 1, line.c_str()[i]);
    }
    return (ret);
}

/* POTION */
void    Logs::track_potions(std::deque<std::string> args)
{
    int idx = 0;
    t_potion    my_potions[] =
    {
        {79633, "Tol'vir - Agility"},
        {79634, "Golemblood - Strength"},
        {79476, "Volcanic - Intel"},
        {78993, "Concentration - Mana"},
        {79475, "Earthen - Armor"},
        {0, ""},
    };

    while (idx < 5)
    {
        if (my_potions[idx].item_id == atoi(args[11].c_str()) && this->on_boss == true)
            std::cout << args[4] << " a utilisé une potion " << my_potions[idx].type << " sur le boss " << this->actual_boss << std::endl;
        idx++;
    }
}
