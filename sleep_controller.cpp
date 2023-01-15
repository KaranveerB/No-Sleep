#include "sleep_controller.h"

SleepController::SleepController() {
    defaultAcSleepOnLidCloseState = getActionOnLidCloseEnabled(PowerType::Ac);
    defaultDcSleepOnLidCloseState = getActionOnLidCloseEnabled(PowerType::Dc);
}

void SleepController::setSleepOnLidClose(bool enabled) {
    if (enabled) {
        setActionOnLidCloseEnabled(PowerType::Ac, defaultAcSleepOnLidCloseState);
        setActionOnLidCloseEnabled(PowerType::Dc, defaultDcSleepOnLidCloseState);
    } else {
        setActionOnLidCloseEnabled(PowerType::Ac, 0);
        setActionOnLidCloseEnabled(PowerType::Dc, 0);
    }
}

unsigned int SleepController::getActionOnLidCloseEnabled(PowerType type) {
#ifdef _WIN32
    POWER_INFORMATION_LEVEL pwrInfoLevel;
    if (type == PowerType::Ac) {
        pwrInfoLevel = SystemPowerPolicyAc;
    } else if (type == PowerType::Dc) {
        pwrInfoLevel = SystemPowerPolicyDc;
    }

    SYSTEM_POWER_POLICY powerPolicy;
    NTSTATUS status = ::CallNtPowerInformation(pwrInfoLevel, (void *) nullptr, 0, &powerPolicy,
                                               sizeof(SYSTEM_POWER_POLICY));
    if (status == STATUS_SUCCESS) {
        return powerPolicy.LidClose.Action;
    } else if (status == STATUS_BUFFER_TOO_SMALL) {
        throw std::runtime_error("buffer too small for system power policy");
    } else if (status == STATUS_ACCESS_DENIED) {
        throw std::runtime_error("insufficient rights to access system power policy");
    } else {
        throw std::runtime_error("unknown error when accessing system power policy");
    }
#else // _WIN32
    throw std::runtime_error("not working on target platform");
#endif
}

void SleepController::setActionOnLidCloseEnabled(PowerType type, unsigned int state) {
#ifdef _WIN32
    POWER_INFORMATION_LEVEL pwrInfoLevel;
    if (type == PowerType::Ac) {
        pwrInfoLevel = SystemPowerPolicyAc;
    } else if (type == PowerType::Dc) {
        pwrInfoLevel = SystemPowerPolicyDc;
    }

    SYSTEM_POWER_POLICY powerPolicy;
    NTSTATUS status = CallNtPowerInformation(pwrInfoLevel, nullptr, 0, &powerPolicy,
                                             sizeof(SYSTEM_POWER_POLICY));
    if (status == STATUS_BUFFER_TOO_SMALL) {
        throw std::runtime_error("buffer too small for system power policy");
    } else if (status == STATUS_ACCESS_DENIED) {
        throw std::runtime_error("insufficient rights to access system power policy");
    } else if (status != STATUS_SUCCESS) {
        throw std::runtime_error("unknown error when accessing system power policy");
    }

    powerPolicy.LidClose.Action = (POWER_ACTION) state;

    status = CallNtPowerInformation(pwrInfoLevel, &powerPolicy, sizeof(SYSTEM_POWER_POLICY), nullptr, 0);
    if (status == STATUS_BUFFER_TOO_SMALL) {
        throw std::runtime_error("buffer too small for system power policy");
    } else if (status == STATUS_ACCESS_DENIED) {
        throw std::runtime_error("insufficient rights to write to system power policy");
    } else if (status != STATUS_SUCCESS) {
        throw std::runtime_error("unknown error to write to system power policy");
    }
#else // _WIN32
    throw std::runtime_error("not working on target platform");
#endif
}

void SleepController::forceSetDefaultLidCloseActionToSleep() {
    defaultAcSleepOnLidCloseState = 1;
    defaultDcSleepOnLidCloseState = 1;
}
