#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ntstatus.h>
#include <powrprof.h>
#include <winternl.h>
#include <winnt.h>

#endif // _WIN32

#include <stdexcept>


class SleepController {

    enum PowerType {
        Ac,
        Dc
    };

public:
    SleepController();

    void setSleepOnLidClose(bool enabled);
    void forceSetDefaultLidCloseActionToSleep();

private:
    static unsigned int getActionOnLidCloseEnabled(PowerType type); // return value > 0 if enabled, 0 if disabled
    static void setActionOnLidCloseEnabled(PowerType type, unsigned int newState);

    // Windows: represents POWER_ACTION (PowerActionNone = 0 so no sleep is same as Linux)
    // Linux: represents boolean value (1 or 0)
    unsigned int defaultAcSleepOnLidCloseState;
    unsigned int defaultDcSleepOnLidCloseState;
};
