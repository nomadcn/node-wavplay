#include <uv.h>
#include <node.h>
#include <stdio.h>
#include <time.h>
#include "WinPlayer.h"

using namespace v8;

#define LOGD(FILTER, ...) 0
#define LOGD_CALL(FILTER, MSG) 0
#define LOGD_LINE(FILTER) 0

static Player* g_player = NULL;

Player* getPlayer() {
    return g_player;
}

void Init(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    if (g_player == NULL)
    {
        g_player = new WinPlayer();
    }

    int ret = (g_player) ? 0 : -1;
    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Open(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    LOGD_CALL(TAG, "end");
}

void Start(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    int ret = -1;
    Isolate* isolate = args.GetIsolate();

    Player* player = getPlayer();
    if (player)
    {
        String::Utf8Value js_filePath(args[0]);
        const char* filePath = *js_filePath;
        const int loop = (int)args[1]->NumberValue();

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

    Player* player = getPlayer();
    if (player)
    {
        ret = player->Stop();
    }

    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Close(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    int ret = -1;

    Player* player = getPlayer();
    if (player)
    {
        ret = player->Close();
    }

    Local<Number> js_ret = Number::New(isolate, ret);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
}

void Exit(const FunctionCallbackInfo<Value>& args)
{
    LOGD_CALL(TAG, "start");

    Isolate* isolate = args.GetIsolate();

    if (g_player)
    {
        delete g_player;
        g_player = NULL;
    }

    Local<Number> js_ret = Number::New(isolate, 0);
    args.GetReturnValue().Set(js_ret);

    LOGD_CALL(TAG, "end");
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
