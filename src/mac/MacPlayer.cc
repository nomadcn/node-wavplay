#include "MacPlayer.h"
#include <gwlog.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>

static int isFileExistent(const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (fp)
    {
        fclose(fp);
    }

    return (fp == NULL) ? 0 : 1;
}

MacPlayer::MacPlayer()
{
}

MacPlayer::~MacPlayer()
{
}

int MacPlayer::Open(const Player::Param* param)
{
    LOGD_CALL(TAG, "start");

    if (param == NULL)
    {
        LOGE_CALL(TAG, "param is null.");
    }
    else if (param->filePath == NULL)
    {
        LOGE_CALL(TAG, "param->filePath is null.");
    }
    else
    {
        if (isFileExistent(param->filePath))
        {
            m_wavFilePath = param->filePath;

            memset(&m_processHandle, 0, sizeof(m_processHandle));

            memset(&m_processOpts, 0, sizeof(m_processOpts));
            m_processOpts.exit_cb = MacPlayer::ExitCb;
            m_processOpts.args = m_processArgs;
            m_processOpts.file = "/usr/bin/afplay";
            m_processArgs[0] = (char*)m_processOpts.file; 
            m_processArgs[1] = (char*)m_wavFilePath.c_str();
            m_processArgs[2] = NULL;
        }
    }

    LOGD_CALL(TAG, "end");

    return 0;
}

void MacPlayer::ExitCb(uv_process_t* process, int64_t exitStatus, int termSignal)
{
    LOGD_CALL(TAG, "start");
    LOGD_CALL(TAG, "end");
}

int MacPlayer::Start(const int loop)
{
    LOGD_CALL(TAG, "start");
    Play(loop);
    LOGD_CALL(TAG, "end");

    return 0;
}

#if 1
void MacPlayer::Play(const int repeat)
{
    uv_loop_t* loop = uv_default_loop();
    LOGD(TAG, "%s(%d) %s: loop(%p)", __CALL_INFO__, loop);

    int ret = uv_spawn(loop, &m_processHandle, &m_processOpts);
    LOGD(TAG, "%s(%d) %s: uv_spawn() returns %d", __CALL_INFO__, ret);
}
#else
void MacPlayer::Play(const int loop)
{
    if (m_wavFilePath.empty())
    {
        LOGW_CALL(TAG, "wav file path is empty.");
        return;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        // child process
        ostringstream oss;
        oss << "afplay " << '"' << m_wavFilePath << '"';

        LOGD(TAG, "%s(%d) %s: wav(%s)", __CALL_INFO__, m_wavFilePath.c_str());

#if 0
        while (loop)
        {
            LOGD_CALL(TAG, oss.str().c_str());
            system(oss.str().c_str());
        }
#else
        execl("/usr/bin/afplay", "afplay", m_wavFilePath.c_str(), NULL);
#endif
    }
    else
    {
        // parent process
        m_pid = pid;
    }
}
#endif

int MacPlayer::Stop()
{
    LOGD_CALL(TAG, "start");

    Kill();

    LOGD_CALL(TAG, "end");
    return 0;
}

void MacPlayer::Kill()
{
    uv_process_kill(&m_processHandle, SIGTERM);
    uv_close((uv_handle_t*)&m_processHandle, NULL);
}

int MacPlayer::Close()
{
    LOGD_CALL(TAG, "start");

    m_wavFilePath.clear();

    LOGD_CALL(TAG, "end");

    return 0;
}
