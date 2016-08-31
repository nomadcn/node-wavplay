#include "Player.h"
#include <string>
#include <State.h>

using namespace std;

class WinPlayer : public Player
{
public:
    WinPlayer();
    virtual ~WinPlayer();

    virtual int Open(const Player::Param* param);
    virtual int Start(const int loop);
    virtual int Stop();
    virtual int Close();

private:
    gw::State m_state;
    wstring m_filePath;
};
