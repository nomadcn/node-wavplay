#include "Player.h"
#include <string>
#include <State.h>
#include <unistd.h>
#include <uv.h>
#include <vector>

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
    void Play();
    void Kill();
    uv_process_t* CreateProcessHandle();
    void CloseProcessHandle(uv_process_t* handle);
    static void ExitCb(uv_process_t* process, int64_t exitStatus, int termSignal);
    static void CloseCb(uv_handle_t* handle);

    int AddProcessHandle(uv_process_t* handle);
    int RemoveProcessHandle(uv_process_t* handle);
    int ClearProcessHandles();

private:
    gw::State m_state;

    // wav filePath to play
    string m_wavFilePath;
    // played repeatedly?
    int m_loop;

    typedef vector<uv_process_t*> HANDLE_VECTOR;
    HANDLE_VECTOR m_processHandles;
    uv_process_t* m_processHandle;

    uv_process_options_t m_processOpts;
    char* m_processArgs[3];
};
