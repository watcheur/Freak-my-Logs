#ifndef LOGS_H
#define LOGS_H

#include "window.h"
#include <QMainWindow>

enum    dragon_soul_boss {
    MORCHOK = 0,
    ZONOZZ,
    YORSAHJ,
    HAGARA,
    ULTRAXION,
    BLACKHORN,
    SPINE,
    MADNESS
};

enum    potion_type {
    AGILITY = 0,
    STRENGTH,
    INTEL,
    MANA,
    ARMOR
};

// UnitFlag bitfield
namespace UnitFLags {
    #define COMBATLOG_OBJECTT_TYPE_MASK             0x0000FC00
    #define COMBATLOG_OBJECTT_TYPE_OBJECT           0x00004000
    #define COMBATLOG_OBJECTT_TYPE_GUARDIAN         0x00002000
    #define COMBATLOG_OBJECTT_TYPE_PET              0x00001000
    #define COMBATLOG_OBJECTT_TYPE_NPC              0x00000800
    #define COMBATLOG_OBJECTT_TYPE_PLAYER           0x00000400
    #define COMBATLOG_OBJECTT_CONTROL_MASK          0x00000300
    #define COMBATLOG_OBJECTT_CONTROL_NPC           0x00000200
    #define COMBATLOG_OBJECTT_CONTROL_PLAYER        0x00000100
    #define COMBATLOG_OBJECTT_REACTION_MASK         0x000000F0
    #define COMBATLOG_OBJECTT_REACTION_HOSTILE      0x00000040
    #define COMBATLOG_OBJECTT_REACTION_NEUTRAL      0x00000020
    #define COMBATLOG_OBJECTT_REACTION_FRIENDLY     0x00000010
    #define COMBATLOG_OBJECTT_AFFILIATION_MASK      0x0000000F
    #define COMBATLOG_OBJECTT_AFFILIATION_OUTSIDER  0x00000008
    #define COMBATLOG_OBJECTT_AFFILIATION_RAID      0x00000004
    #define COMBATLOG_OBJECTT_AFFILIATION_PARTY     0x00000002
    #define COMBATLOG_OBJECTT_AFFILIATION_MINE      0x00000001
    #define COMBATLOG_OBJECTT_SPECIAL_MASK          0xFFFF0000
    #define COMBATLOG_OBJECTT_NONE                  0x80000000
    #define COMBATLOG_OBJECTT_RAIDTARGET8           0x08000000
    #define COMBATLOG_OBJECTT_RAIDTARGET7           0x04000000
    #define COMBATLOG_OBJECTT_RAIDTARGET6           0x02000000
    #define COMBATLOG_OBJECTT_RAIDTARGET5           0x01000000
    #define COMBATLOG_OBJECTT_RAIDTARGET4           0x00800000
    #define COMBATLOG_OBJECTT_RAIDTARGET3           0x00400000
    #define COMBATLOG_OBJECTT_RAIDTARGET2           0x00200000
    #define COMBATLOG_OBJECTT_RAIDTARGET1           0x00100000
    #define COMBATLOG_OBJECTT_MAINASSIST            0x00080000
    #define COMBATLOG_OBJECTT_MAINTANK              0x00040000
    #define COMBATLOG_OBJECTT_FOCUS                 0x00020000
    #define COMBATLOG_OBJECTT_TARGET                0x00010000
}

// Boss tracking
typedef struct  s_boss {
    std::string specific_tracking_a;
    std::string specific_tracking_b;
    std::string name_fr;
    std::string name_en;
}               t_boss;

// Potion tracking
typedef struct  s_potion {
    int item_id;
    std::string type;
}               t_potion;

typedef struct  s_used {
    int type;
    std::string name;
    std::string boss;
}               t_used;

class Logs : public Window
{
    Q_OBJECT
public:
    explicit Logs(std::string filename);
    ~Logs();
    
private:
    // Var
    bool            on_boss;
    std::string     actual_boss;
    std::ifstream   filedes;
    std::deque<std::string>  players;
    std::deque<std::string>    boss_logs;

    // Functions
    std::deque<std::string> parse_log(std::string line);
    std::string get_args(const char *content, int *pos);

    // Boss tracking
    void    track_boss(std::deque<std::string> args);

    // Potions tracking
    void potion_resum();
    std::string track_potions(std::deque<std::string>);
    std::deque<std::string> agility;
    std::deque<std::string> strength;
    std::deque<std::string> intelligence;
    std::deque<std::string> mana;
    std::deque<std::string> armor;

    std::deque<t_used>  potion_track;

    // Death tracking

    // Rez tracking
};

#endif // LOGS_H
