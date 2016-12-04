//
// Created by adam on 29.11.2016.
//

#ifndef CLION_VARIABLES_H
#define CLION_VARIABLES_H


/**
 * EEPROM shifting address
 */
int eeShift = 50;
int eeDayLight = 0;
int eeEvenLight = eeDayLight + eeShift;
int eeMisting = eeEvenLight + eeShift;
int eeFoog = eeMisting + eeShift;


#endif //CLION_VARIABLES_H
