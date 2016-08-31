#include "wavplay.h"

/**
 * Basic Player class
 */
class Player
{
public:
    struct Param
    {
        // the path of wav file to play.
        const char* filePath;
    };

    Player() {}
    virtual ~Player() {}

    virtual int Open(const Param* param) = 0;
    /**
     * Play sound asynchronously.
     */
    virtual int Start(const int loop) = 0;
    virtual int Stop() = 0;
    virtual int Close() = 0;
};
