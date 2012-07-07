#ifndef LOGS_H
#define LOGS_H

#include "window.h"
#include <QMainWindow>

class Logs : public Window
{
    Q_OBJECT
public:
    explicit Logs(std::string filename);
    ~Logs();
    
private:
    bool            on_boss;
    std::string     actual_boss;
    std::ifstream   filedes;
    std::deque<std::string> parse_log(std::string line);
    std::string get_args(const char *content, int *pos);

    // Boss tracking
    typedef struct  s_boss {
        std::string specific_tracking_a;
        std::string specific_tracking_b;
        std::string name_fr;
        std::string name_en;
    }               t_boss;

    void    track_boss(std::deque<std::string> args);

    // Potions tracking
    void track_potions(std::deque<std::string>);
    typedef struct  s_potion {
        int item_id;
        std::string type;
    }               t_potion;

    typedef struct  s_used {
        int type;
        std::string name;
        std::string boss;
    }               t_used;

    std::deque<t_used>  potion_track;

    // Death tracking

    // Rez tracking
};

#endif // LOGS_H
