#include "display_globals.h"
#include <SPI.h>

// Barramento compartilhado (TFT + SD)
SPIClass spiTFT_SD(VSPI);

// Instanciação do objeto TFT usando TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

// SPI bus dedicado para o Touch (HSPI na CYD)
SPIClass touchSPI(HSPI);

// Instanciação do Touch (XPT2046)
XPT2046_Touchscreen touch(PIN_TOUCH_CS);

// Instanciação do SD
SdFat sd;

// Mutex para sincronização de acesso ao display (FreeRTOS)
SemaphoreHandle_t g_tft_mutex = xSemaphoreCreateRecursiveMutex();
