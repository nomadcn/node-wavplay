#include <uv.h>
#include <node.h>
#include <stdio.h>
#include <time.h>
#include <gwlog.h>
#include <string>

#ifdef WIN32
#include <WinPlayer.h>
#else
#include <MacPlayer.h>
#endif

using namespace v8;
using namespace std;

#if 0
#define LOGD(FILTER, ...) 0
#define LOGD_CALL(FILTER, MSG) 0
#define LOGD_LINE(FILTER) 0
#endif

static Player* g_player = NULL;

static Player* GetPlayer()
{
    return g_player;
}

static Player* CreatePlayer()
{
    if (g_player == NULL)
    {
#ifdef WIN32
        g_player = new WinPlayer();
#else
        g_player = new MacPlayer();
#endif
    }

    return g_player;
}

static void ReleasePlayer()
{
    if (g_player)
    {
        delete g_player;
        g_player = NULL;
    }
}

// -------------------------------------------------------------------------

void Init(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    string logPath;
    int flags = eLO_FILE;
    int logLevel = eLL_DEBUG;

    const int argc = args.Length();
    if (argc > 0)
    {
        String::Utf8Value js_filePath(args[0]);
        logPath = *js_filePath;
    }
    if (argc > 1)
    {
        flags = (int)args[1]->NumberValue();
    }
    if (argc > 2)
    {
        logLevel = (int)args[2]->NumberValue();
    }

    gwlog_init(logPath.c_str(), flags, logLevel);

    Player* player = CreatePlayer();

    int ret = (player) ? 0 : -1;
    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Open(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    Local<Number> js_ret = Number::New(isolate, 0);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Start(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = -1;
    Isolate* isolate = args.GetIsolate();

    Player* player = GetPlayer();
    if (player)
    {
        String::Utf8Value js_filePath(args[0]);
        const char* filePath = *js_filePath;
        const int loop = (int)args[1]->NumberValue();
        LOGD(TAG, "%s(%d) %s: filePath(%s) loop(%d)",
            __CALL_INFO__, filePath, loop);

        Player::Param param;
        param.filePath = filePath;

        ret = player->Open(&param);
        if (ret == 0)
        {
            ret = player->Start(loop);
        }
    }

    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Stop(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    int ret = -1;

    Player* player = GetPlayer();
    if (player)
    {
        ret = player->Stop();
        ret = player->Close();
    }

    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Close(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    Local<Number> js_ret = Number::New(isolate, 0);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Exit(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    ReleasePlayer();

    Local<Number> js_ret = Number::New(isolate, 0);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");

    gwlog_exit();
}

/*
 * addon initialzation
 */
void init(Handle<Object> exports)
{
    LOGD_CALL(TAG, "start");

    NODE_SET_METHOD(exports, "init", Init);
    NODE_SET_METHOD(exports, "open", Open);
    NODE_SET_METHOD(exports, "start", Start);
    NODE_SET_METHOD(exports, "stop", Stop);
    NODE_SET_METHOD(exports, "close", Close);
    NODE_SET_METHOD(exports, "exit", Exit);

    LOGD_CALL(TAG, "end");
}

NODE_MODULE(win_wavplay, init)
