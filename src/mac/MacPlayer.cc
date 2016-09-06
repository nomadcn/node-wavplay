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

MacPlayer::MacPlayer() :
m_pid(0)
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
        }
    }

    LOGD_CALL(TAG, "end");

    return 0;
}

int MacPlayer::Start(const int loop)
{
    Stop();

    LOGD_CALL(TAG, "start");
    Play(loop);
    LOGD_CALL(TAG, "end");

    return 0;
}

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

int MacPlayer::Stop()
{
    LOGD_CALL(TAG, "start");

    if (m_pid > 0)
    {
        LOGI(TAG, "%s(%d) %s: kill pid(%d)", __CALL_INFO__, m_pid);
        kill(m_pid, SIGKILL);
        kill(m_pid+1, SIGKILL);
        m_pid = 0;
    }

    LOGD_CALL(TAG, "end");
    return 0;
}

int MacPlayer::Close()
{
    Stop();

    LOGD_CALL(TAG, "start");
    m_wavFilePath.clear();
    LOGD_CALL(TAG, "end");

    return 0;
}
