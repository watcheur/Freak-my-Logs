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

// UnitFlag bitfield
// See: http://www.wowpedia.org/UnitFlag
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

// Boss tracker
namespace Boss {
    typedef struct  s_boss {
        std::string specific_tracking_a;
        std::string specific_tracking_b;
        std::string name_fr;
        std::string name_en;
    }               t_boss;

    // Dragon Soul Boss
    namespace DragonSoul {
        enum    Boss {
            MORCHOK = 0,
            ZONOZZ,
            YORSAHJ,
            HAGARA,
            ULTRAXION,
            BLACKHORN,
            SPINE,
            MADNESS
        };
    }
}

// Potion tracker
namespace Potion {
    // Define pouvant être utile
    #define AGILITY_POTION  79633 // ID du spell de la potion d'agilité
    #define STRENGTH_POTION 79634 // ID du spell de la potion de force
    #define INTEL_POTION    79476 // ID du spell de la potion d'intelligence
    #define REGEN_POTION    78993 // ID du spell de la potion de regen mana
    #define ARMOR_POTION    79475 // ID du spell de la potion d'armure

    // Structure pour la création d'un tableau avec les ID et les noms de potions
    typedef struct  s_potion {
        int item_id;
        std::string type;
    }               t_potion;

    // Enum des différents type de potions dans la structure (voir ci-dessus)
    enum    potion_type {
        AGILITY = 0,
        STRENGTH,
        INTEL,
        MANA,
        ARMOR
    };
}

// Class du Parser
// Hérite de la classe qui gère l'application
class Logs : public Window
{
    Q_OBJECT
public:
    explicit Logs(std::string filename);    // Constructeur
    ~Logs();    // Destructeur
    
private:
    // Boss Variables
    bool            on_boss;        // Un boss est-il en cours ? (True / False)
    std::string     actual_boss;    // Quel est le nom du boss ?
    std::deque<std::string>    boss_logs;   // Logs des différents boss, démarre avec: _name engaged finit par _name died.

    // Parser Variables
    std::ifstream   filedes;        // File descriptor du combatlog

    // Players Variables
    std::deque<std::string>  players;   //  Joueurs présents dans le raid

    // Potion Variables
    std::deque<std::string> agility;        // Joueurs ayant utilisé une potion d'agilité
    std::deque<std::string> strength;       // Joueurs ayant utilisé une potion de force
    std::deque<std::string> intelligence;   // Joueurs ayant utilisé une potion d'intelligence
    std::deque<std::string> mana;           // Joueurs ayant utilisé une potion de mana
    std::deque<std::string> armor;          // Joueurs ayant utilisé une potion d'armure

    // Parser Functions
    std::deque<std::string> parse_log(std::string line);    // Parser ligne par ligne du combatlog
    std::string get_args(const char *content, int *pos);    // Parser des arguments des lignes du combatlog

    // Boss tracker functions
    void    track_boss(std::deque<std::string> args);       // Tracker du boss actuel (Pull / Mort)

    // Potions tracker functions
    std::string track_potions(std::deque<std::string>);     // Tracker des potions utilisés en raid
    void potion_resum();                                    // Récapitulatif des potions en fonction du type

    // Death tracker functions

    // Rez tracker functions
};

#endif // LOGS_H
