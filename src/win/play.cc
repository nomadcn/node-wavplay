/**
 * wav play on windows
 */
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <mmsystem.h>

int PlaySound(const TCHAR* fileName)
{
    int ret = PlaySound(fileName, NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
    printf("ret: %d\n", ret);

    return ret;
}
