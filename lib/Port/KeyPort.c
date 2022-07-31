#include "KeyPort.h"
#include "KeyIO.h"

#include <avr/io.h>


#if KEY_HW == KEY_HW_AVR

void Key_initPin(const Key_PinConfig* config) {
    config->IO->Direction.Value &= ~config->Pin;
    config->IO->OutputData.Value |= config->Pin;
}
uint8_t Key_readPin(const Key_PinConfig* config) {
    return (config->IO->InputData.Value & config->Pin) != 0;
}
#if KEY_USE_DEINIT
void Key_deInitPin(const Key_PinConfig* config) {
    config->IO->OutputData.Value &= ~config->Pin;
}
#endif

#endif // KEY_HW



