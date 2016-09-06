#include "Player.h"
#include <string>
#include <State.h>
#include <unistd.h>

using namespace std;

class MacPlayer : public Player
{
public:
    MacPlayer();
    virtual ~MacPlayer();

    virtual int Open(const Player::Param* param);
    virtual int Start(const int loop);
    virtual int Stop();
    virtual int Close();

private:
    void Play(const int loop);

private:
    // wav filePath to play
    string m_wavFilePath;
    // process id of afplay
    pid_t m_pid;
};
