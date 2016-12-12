//
// Created by adam on 12.12.2016.
//

#ifndef CLION_TERARIUMDAO_H
#define CLION_TERARIUMDAO_H


#include <TerraController.h>
#include "EEPROM.h"

class TerariumDao {
public:
    void createTC(TerraController terraController);
    void updateTC(TerraController terraController);
    void deleteTC(TerraController terraController);
    TerraController readTC(TerraController terraController);
private:
    static int eeShift = 50;
    static int eeTerraControllerIndex = 50;
    static int eeEvenLight = eeTerraControllerIndex + eeShift;
    static int eeMisting = eeEvenLight + eeShift;
    int eeFoog = eeMisting + eeShift;
};


#endif //CLION_TERARIUMDAO_H
