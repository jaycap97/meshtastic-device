#include "kbI2cBase.h"
#include "configuration.h"
#include <Wire.h>

extern uint8_t cardkb_found;

KbI2cBase::KbI2cBase(const char *name) : concurrency::OSThread(name)
{
    this->_originName = name;
}

int32_t KbI2cBase::runOnce()
{
    InputEvent e;
    e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_NONE;
    e.source = this->_originName;

    Wire.requestFrom(CARDKB_ADDR, 1);

    while (Wire.available()) {
        char c = Wire.read();
        switch (c) {
        case 0x1b: // ESC
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_CANCEL;
            break;
        case 0x08: // Back
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_BACK;
            break;
        case 0xb5: // Up
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_UP;
            break;
        case 0xb6: // Down
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_DOWN;
            break;
        case 0xb4: // Left
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_LEFT;
            break;
        case 0xb7: // Right
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT;
            break;
        case 0x0d: // Enter
            e.inputEvent = ModuleConfig_CannedMessageConfig_InputEventChar_SELECT;
            break;
        }
    }

    if (e.inputEvent != ModuleConfig_CannedMessageConfig_InputEventChar_NONE) {
        this->notifyObservers(&e);
    }
    return 500;
}
