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
    std::ifstream filedes;
    std::deque<std::string> parse_log(std::string line);
    std::string get_args(const char *content, int *pos);

    // Potions tracking
    void track_potions(std::deque<std::string>);
    typedef struct  s_potion {
        int item_id;
        std::string type;
    }               t_potion;

    // Death tracking

    // Rez tracking
};

#endif // LOGS_H
