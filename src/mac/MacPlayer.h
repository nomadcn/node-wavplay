#include "Player.h"
#include <string>
#include <State.h>

using namespace std;

class MacPlayer : public Player
{
    MacPlayer();
    virtual ~MacPlayer();

    virtual int Open(const Player::Param* param);
    virtual int Start(const int loop);
    virtual int Stop();
    virtual int Close();
};
