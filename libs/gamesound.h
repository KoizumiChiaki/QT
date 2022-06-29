#ifndef GAMESOUND_H
#define GAMESOUND_H
#include <QSound>
namespace __gameSound {
    class soundState
    {
    public:
        bool crash, shoot, jump, fall, dead;
        //bullet crash
        //shoot bullet
        //jump
        //fall
        void soundInit()
        {
            crash = false;
            shoot = false;
            jump = false;
            fall = false;
            dead = false;
        }
    }soundStatus;
}
using __gameSound::soundStatus;
#endif // GAMESOUND_H
