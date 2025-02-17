#pragma once

#include <format>
#include "../../../Events/Listener.hpp"
#include "../../../Events/Input/KeyEvent.hpp"
#include "../../../../Utils/Logger/Logger.hpp"
#include "../Module.hpp"
#include "../../../../SDK/SDK.hpp"
#include <Windows.h>

class NoHurtCamListener : public Listener {
    Module* module;

    // 76 1C 66 0F 6E 00 | 40 F2 0F 11 03 8B 44 24 48 89 43 08 C6 43 0C 01
    // 8B 44 24 48 89 43 08 C6 patch to 8B 44 24 48 90 90 90 C6

    static inline bool patched = false;

    static inline uintptr_t sigOffset;
    static inline std::vector<uint8_t> originalCameraAngle;

public:

    static void patch() {
        if(patched) return;
        patched = true;
        int size;
        if (WinrtUtils::checkAboveOrEqual(21, 30)) {
            size = 5;
        } else {
            size = 3;
        }
        Memory::nopBytes((LPVOID) sigOffset, size);
    }

    static void unpatch() {
        if(!patched) return;
        patched = false;
        int size;
        if (WinrtUtils::checkAboveOrEqual(21, 30)) {
            size = 5;
        } else {
            size = 3;
        }
        Memory::patchBytes((LPVOID) sigOffset, originalCameraAngle.data(), size);
    }

    void onRaknetTick(RaknetTickEvent &event) override {
        if (module->isEnabled()) {
            std::string serverIP = SDK::getServerIP();
            module->restricted = false;
        }
    }

    void onTick(TickEvent &event) override {
        if (!module->restricted) {
            patch();
        } else {
            unpatch();
        }
    }

    void onUnregister() override {
        unpatch();
    }

    NoHurtCamListener(const char string[5], Module *module) {
        this->name = string;
        this->module = module;

        int size;
        if (WinrtUtils::checkAboveOrEqual(21, 30)) {
            size = 5;
        } else {
            size = 3;
        }

        originalCameraAngle.resize(size);

        if(sigOffset == NULL) {
            if (WinrtUtils::checkAboveOrEqual(21, 30)) {
                sigOffset = GET_SIG_ADDRESS("CameraAssignAngle");
            } else {
                sigOffset = GET_SIG_ADDRESS("CameraAssignAngle") + 4;
            }
        }

        Memory::patchBytes( originalCameraAngle.data(), (LPVOID)sigOffset, size);
    }
};
