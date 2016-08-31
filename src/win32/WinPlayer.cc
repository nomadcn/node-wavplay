#include "WinPlayer.h"
#include <Windows.h>
#include <mmsystem.h>
#include <CharsetUtil.h>

WinPlayer::WinPlayer() :
m_state(gw::eLCS_INIT)
{
}

WinPlayer::~WinPlayer()
{
}

int WinPlayer::Open(const Player::Param* param)
{
    Stop();
    Close();

    wstring filePath = gw::Utf8ToUnicode(param->filePath);
    if (filePath.empty()) {
        return -1;
    }

    m_filePath = filePath;
    m_state.SetState(gw::eLCS_OPEN);
}

int WinPlayer::Start(const int loop)
{
    if (m_state == gw::eLCS_INIT)
    {
        return -1;
    }

    uint32_t options = SND_FILENAME | SND_ASYNC;
    if (loop)
    {
        options |= SND_LOOP;
    }

    int ret = PlaySound(m_filePath.c_str(), NULL, options);

    if (ret)
    {
        m_state.SetState(gw::eLCS_START);
    }

    return ret;
}

int WinPlayer::Stop()
{
    if (m_state != gw::eLCS_START)
    {
        return -1;
    }

    int ret = PlaySound(NULL, NULL, 0);

    m_state.SetState(gw::eLCS_OPEN);

    return ret;
}

int WinPlayer::Close()
{
    if (m_state == gw::eLCS_INIT)
    {
        return 0;
    }

    Stop();

    m_filePath.clear();

    m_state.SetState(gw::eLCS_INIT);

    return 0;
}
