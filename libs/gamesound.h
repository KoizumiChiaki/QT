#ifndef GAMESOUND_H
#define GAMESOUND_H
#include <QSound>
namespace __gameSound {
    class soundState
    {
    public:
        bool crash;
        bool shoot;
        bool jump;
        void soundInit()
        {
            crash = false;
            shoot = false;
            jump = false;
        }
    }soundStatus;
}
using __gameSound::soundStatus;
#endif // GAMESOUND_H
