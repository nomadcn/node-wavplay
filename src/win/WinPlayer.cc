#include "WinPlayer.h"
#include <Windows.h>
#include <mmsystem.h>
#include <gwlog.h>

static int Utf8ToUnicode(const string& strUtf8, wstring& out)
{
    const char* inBuf = strUtf8.c_str();
    const int inSize = (int)strUtf8.size() + 1;

    // Get the size of characters which converted string has.
	int ret = MultiByteToWideChar(
        CP_UTF8, 0, inBuf, inSize,
        NULL, 0);
    LOGD(TAG, "%s(%d) %s: string length(%d)", __CALL_INFO__, ret);

	if (ret <= 0)
	{
		return -1;
	}

	const int outSize = ret;
	wchar_t* outBuf = new wchar_t [outSize];

	ret = MultiByteToWideChar(
        CP_UTF8, 0, inBuf, inSize,
        outBuf, outSize);
    if (ret > 0)
    {
        outBuf[ret - 1] = _T('\0');
        out = outBuf;
        delete[] outBuf;
    }

	return ret;
}

WinPlayer::WinPlayer() :
m_state(gw::eLCS_INIT)
{
    LOGD_LINE(TAG);
}

WinPlayer::~WinPlayer()
{
    LOGD_CALL(TAG, "start");
    Close();
    LOGD_CALL(TAG, "end");
}

int WinPlayer::Open(const Player::Param* param)
{
    Stop();
    Close();

    LOGD(TAG, "start path(%s)", param->filePath);

    wstring filePath;
    int ret = Utf8ToUnicode(param->filePath, filePath);
    if (ret < 1) {
        return -1;
    }

    m_filePath = filePath;
    m_state.SetState(gw::eLCS_OPEN);

    LOGD_CALL(TAG, "end");

    return 0;
}

int WinPlayer::Start(const int loop)
{
    LOGD(TAG, "%s(%d) %s: start loop(%d)", __CALL_INFO__, loop);

    if (m_state == gw::eLCS_INIT)
    {
        LOGW(TAG, "%s(%d) %s wrong state: %s -> start",
            __CALL_INFO__, m_state.GetStateName());
        return -1;
    }

    uint32_t options = SND_FILENAME | SND_ASYNC;
    if (loop)
    {
        options |= SND_LOOP;
    }

    int ret = PlaySound(m_filePath.c_str(), NULL, options);
    LOGD(TAG, "%s(%d) %s PlaySound() returns %d", __CALL_INFO__, ret);

    if (ret)
    {
        m_state.SetState(gw::eLCS_START);
    }

    LOGD_CALL(TAG, "end");

    return (ret) ? 0 : -2;
}

int WinPlayer::Stop()
{
    LOGD_CALL(TAG, "start");

    if (m_state == gw::eLCS_START)
    {
        int ret = PlaySound(NULL, NULL, 0);
        LOGD(TAG, "%s(%d) %s PlaySound() returns %d", __CALL_INFO__, ret);
        m_state.SetState(gw::eLCS_OPEN);
    }

    LOGD_CALL(TAG, "end");

    return 0;
}

int WinPlayer::Close()
{
    LOGD_CALL(TAG, "start");

    if (m_state != gw::eLCS_INIT)
    {
        Stop();
        m_filePath.clear();
        m_state.SetState(gw::eLCS_INIT);
    }

    LOGD_CALL(TAG, "end");

    return 0;
}
