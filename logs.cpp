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
    (void)filename;
    std::string line;
    std::deque<std::string> args;
    /*
    this->filedes.open(filename.c_str(), std::ifstream::in);
    if (filedes.is_open())
    {
        int i = 0;
        while (filedes.good() && i < 1)
        {
            getline(filedes, line);
            args = parse_log(line);
            track_potions(args);
            i++;
        }
    }
    else
        std::cout << "Unable to open file" << std::endl;
        */
    line = "12/28 21:02:23.629  SPELL_AURA_APPLIED,0x0680000002F548FA,\"Rhogar\",0x511,0x0,0x0680000002F548FA,\"Rhogar\",0x511,0x0,79633,\"Agilit des Tol'vir\",0x10,BUFF";
    args = parse_log(line);
    track_potions(args);
}

Logs::~Logs()
{
    filedes.close();
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

void    Logs::track_potions(std::deque<std::string> args)
{
    /*
    t_potion    potions[] =
    {
        {79633, "Tol'vir - Agility"},
        {79634, "Golemblood - Strength"},
        {79476, "Volcanic - Intel"},
        {78993, "Concentration - Mana"},
        {79475, "Earthen - Armor"},
        {0, NULL},
    };
    */

    for (int i  = 0; i < (int)args.size(); ++i)
        std::cout << i << ":" << args[i] << std::endl;
}
