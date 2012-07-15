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
#define PLAYER_IN_MY_RAID                       0xFFFF0802

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

    void potion_resum();
    void track_spell(std::deque<std::string>);

    void track_boss(std::deque<std::string> args);

private slots:
    void launch_parse();

private:
    // Interface
    std::string     filename;
    QPushButton     *createButton(const QString &text, const char *member);
    QGridLayout     *parseLayout;
    QPushButton     *parseButton;

    QLabel          *bossLabel;
    QLineEdit       *trackedBoss;

    QLabel          *spellsLabel;
    QLineEdit       *trackedSpells;

    // Parser
    std::ifstream   filedes;
    std::string actual_boss;
    bool on_boss;
    std::deque<std::string> logs;

    // Players
    std::deque<std::string> players;

    // Boss
    std::deque<std::string> boss_name;

    // Spells
    std::deque<int> spells_id;
};

#endif // LOGS_H
