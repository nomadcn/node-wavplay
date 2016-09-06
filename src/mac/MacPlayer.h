#include "Player.h"
#include <string>
#include <State.h>
#include <unistd.h>
#include <uv.h>

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
    void Play(const int repeat);
    void Kill();
    static void ExitCb(uv_process_t* process, int64_t exitStatus, int termSignal);

private:
    // wav filePath to play
    string m_wavFilePath;

    uv_process_t m_processHandle;
    uv_process_options_t m_processOpts;
    char* m_processArgs[3];
};
