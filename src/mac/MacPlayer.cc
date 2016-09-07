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
m_state(gw::eLCS_INIT)
,m_processHandle(NULL)
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

            memset(&m_processOpts, 0, sizeof(m_processOpts));
            m_processOpts.exit_cb = MacPlayer::ExitCb;
            m_processOpts.args = m_processArgs;
            m_processOpts.file = "/usr/bin/afplay";
            m_processArgs[0] = (char*)m_processOpts.file; 
            m_processArgs[1] = (char*)m_wavFilePath.c_str();
            m_processArgs[2] = NULL;

            m_state.SetState(gw::eLCS_OPEN);
        }
    }

    LOGD_CALL(TAG, "end");

    return 0;
}

uv_process_t* MacPlayer::CreateProcessHandle()
{
    uv_process_t* handle = (uv_process_t*)malloc(sizeof(uv_process_t));
    memset(handle, 0, sizeof(uv_process_t));
    handle->data = this;

    return handle;
}

/**
 * When player process is over, this callback is called on main thread.
 * @param process
 * @param exitStatus
 * @param termSignal
 */
void MacPlayer::ExitCb(uv_process_t* handle, int64_t exitStatus, int termSignal)
{
    LOGD(TAG, "%s(%d) %s: start handle(%p) pid(%d) exitStatus(%lld) signal(%d)",
        __CALL_INFO__, handle, handle->pid, exitStatus, termSignal);

    MacPlayer* _this = (MacPlayer*)handle->data;
    if (_this == NULL)
    {
        LOGE_CALL(TAG, "handle->data is null.");
        return;
    }

    _this->CloseProcessHandle(handle);

    if (_this->m_state == gw::eLCS_START && _this->m_loop)
    {
        _this->Play();
    }

    LOGD_CALL(TAG, "end");
}

void MacPlayer::CloseCb(uv_handle_t* handle)
{
    LOGD(TAG, "%s(%d) %s: start handle(%p)", __CALL_INFO__, handle);

    MacPlayer* _this = (MacPlayer*)handle->data;
    _this->RemoveProcessHandle((uv_process_t*)handle);

    LOGD_CALL(TAG, "end");
}

void MacPlayer::CloseProcessHandle(uv_process_t* handle)
{
    LOGD(TAG, "%s(%d) %s: start handle(%p) pid(%d)", __CALL_INFO__, handle, handle->pid);

    if (handle == NULL)
    {
        LOGD_CALL(TAG, "end");
        return;
    }

    if (handle != m_processHandle)
    {
        LOGW(TAG, "%s(%d) %s: handle(%p) != m_processHandle(%p)",
            __CALL_INFO__, handle, m_processHandle);
    }

    LOGD(TAG, "%s(%d) %s: handle->flags(%d)", __CALL_INFO__, handle->flags);
    uv_close((uv_handle_t*)handle, MacPlayer::CloseCb);
    LOGD(TAG, "%s(%d) %s: handle->flags(%d)", __CALL_INFO__, handle->flags);

    LOGD_CALL(TAG, "end");
}

int MacPlayer::Start(const int loop)
{
    LOGD_CALL(TAG, "start");

    if (m_state == gw::eLCS_START)
    {
        // stop the previous play.
        Stop();
    }

    if (m_state != gw::eLCS_OPEN)
    {
        LOGW(TAG, "%s(%d) %s wrong state: %s -> start",
            __CALL_INFO__, m_state.GetStateName());
        return -1;
    }

    m_loop = loop;
    Play();
    m_state.SetState(gw::eLCS_START);

    LOGD_CALL(TAG, "end");

    return 0;
}

/**
 * fork a process for afplay to play a wav file.
 */
void MacPlayer::Play()
{
    LOGD_CALL(TAG, "start");

    uv_loop_t* loop = uv_default_loop();
    LOGD(TAG, "%s(%d) %s: loop(%p)", __CALL_INFO__, loop);

    uv_process_t* handle = CreateProcessHandle();

    int ret = uv_spawn(loop, handle, &m_processOpts);
    LOGD(TAG, "%s(%d) %s: uv_spawn() ret(%d) handle(%p) pid(%d)",
        __CALL_INFO__, ret, handle, handle->pid);

    if (ret != 0)
    {
        LOGE(TAG, "%s(%d) %s: Failed to play file(%s)",
            __CALL_INFO__, m_wavFilePath.c_str(), ret);
        free(handle);
    }
    else
    {
        AddProcessHandle(handle);
    }

    LOGD_CALL(TAG, "end");
}

int MacPlayer::Stop()
{
    LOGD_CALL(TAG, "start");

    if (m_state != gw::eLCS_START)
    {
        LOGW(TAG, "%s(%d) %s wrong state: %s -> stop",
            __CALL_INFO__, m_state.GetStateName());
        return -1;
    }

    Kill();
    m_state.SetState(gw::eLCS_OPEN);

    LOGD_CALL(TAG, "end");
    return 0;
}

void MacPlayer::Kill()
{
    LOGD_CALL(TAG, "start");

    if (m_processHandle)
    {
        LOGI(TAG, "%s(%d) %s: handle(%p) pid(%d) is killed.",
            __CALL_INFO__, m_processHandle, m_processHandle->pid);
        uv_process_kill(m_processHandle, SIGTERM);

        CloseProcessHandle(m_processHandle);
    }

    LOGD_CALL(TAG, "end");
}

int MacPlayer::Close()
{
    LOGD_CALL(TAG, "start");

    if (m_state != gw::eLCS_OPEN)
    {
        LOGW(TAG, "%s(%d) %s wrong state: %s -> close",
            __CALL_INFO__, m_state.GetStateName());
        return -1;
    }

    ClearProcessHandles();
    
    m_wavFilePath.clear();
    m_state.SetState(gw::eLCS_INIT);

    LOGD_CALL(TAG, "end");

    return 0;
}


// --------------------------

int MacPlayer::AddProcessHandle(uv_process_t* handle)
{
    m_processHandles.push_back(handle);
    m_processHandle = handle;

    LOGD(TAG, "%s(%d) %s: processHandles(%d)", __CALL_INFO__, m_processHandles.size());

    return 0;
}

int MacPlayer::RemoveProcessHandle(uv_process_t* handle)
{
    HANDLE_VECTOR::iterator it = m_processHandles.begin();
    HANDLE_VECTOR::iterator itEnd = m_processHandles.end();

    while (it != itEnd)
    {
        if (*it == handle)
        {
            m_processHandles.erase(it);
            free(handle);
            break;
        }

        ++it;
    }

    const size_t size = m_processHandles.size();
    if (size == 0)
    {
        m_processHandle = NULL;
    }

    LOGD(TAG, "%s(%d) %s: processHandles(%d)", __CALL_INFO__, size);
    return 0;
}

int MacPlayer::ClearProcessHandles()
{
    const int size = m_processHandles.size();

    for (int i = 0; i < size; i++)
    {
        free(m_processHandles[i]);
    }

    m_processHandles.clear();

    LOGD(TAG, "%s(%d) %s: processHandles(%d)", __CALL_INFO__, m_processHandles.size());

    return 0;
}
