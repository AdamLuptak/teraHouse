//
// Created by adam on 12.12.2016.
//

#include "TerariumDao.h"

void TerariumDao::createTC(TerraController terraController) {
    EEPROM.put(eeTerraControllerIndex, terraController);
}

void TerariumDao::updateTC(TerraController terraController) {
    EEPROM.put(eeTerraControllerIndex, terraController);
}

void TerariumDao::deleteTC(TerraController terraController) {
}

TerraController TerariumDao::readTC(TerraController terraController) {
    EEPROM.read(eeTerraControllerIndex);
}
