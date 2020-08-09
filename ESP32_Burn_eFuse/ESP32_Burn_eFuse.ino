// Burn eFuse to Disable MTDI strapping check at startup
// You only have to run it once on each ESP32 module; this is a PERMANENT solution.

/*
  NOTE: eFuse is One-time programmable, it can only be changed once.
  If these fuse bits get set incorrectly, you will permanently brick your ESP32 module.
  Use this suggestion at YOUR own risk.

  references --
  *https://github.com/espressif/esptool/wiki/ESP32-Boot-Mode-Selection
  *https://github.com/espressif/esp-idf/tree/release/v3.2/examples/storage/sd_card#note-about-gpio12
  *https://github.com/espressif/arduino-esp32/issues/3621
*/

// libraries required to work with eFuses
#include "soc/efuse_reg.h"
#include "esp_efuse.h" // for programming eFuse.

void setup() {
  // INITIAL SETUP...BURN THE EFUSE IF NECESSARY FOR PROPER OPERATION.
  // Force the FLASH voltage regulator to 3.3v, disabling "MTDI" strapping check at startup
  if ((REG_READ(EFUSE_BLK0_RDATA4_REG) & EFUSE_RD_SDIO_TIEH) == 0) {
    esp_efuse_reset();  //burning SDIO_TIEH -> sets SDIO voltage regulator to pass-thru 3.3v from VDD
    REG_WRITE(EFUSE_BLK0_WDATA4_REG, EFUSE_RD_SDIO_TIEH);
    esp_efuse_burn_new_values();
  }
  if ((REG_READ(EFUSE_BLK0_RDATA4_REG) & EFUSE_RD_XPD_SDIO_REG) == 0) {
    esp_efuse_reset();  //burning SDIO_REG -> enables SDIO voltage regulator (otherwise user must hardwire power to SDIO)
    REG_WRITE(EFUSE_BLK0_WDATA4_REG, EFUSE_RD_XPD_SDIO_REG);
    esp_efuse_burn_new_values();
  }
  if ((REG_READ(EFUSE_BLK0_RDATA4_REG) & EFUSE_RD_SDIO_FORCE) == 0) {
    esp_efuse_reset();  //burning SDIO_FORCE -> enables SDIO_REG and SDIO_TIEH
    REG_WRITE(EFUSE_BLK0_WDATA4_REG, EFUSE_RD_SDIO_FORCE);
    esp_efuse_burn_new_values();
  }
}

void loop() {

}
