//
// Created by yauta on 26.03.2020.
//

#ifndef SIM_PEDALS_CALIBRATION_H
#define SIM_PEDALS_CALIBRATION_H

void readEEPROM(unsigned int leftBrakeMax,unsigned int rightBrakeMax,unsigned int yawMin,unsigned int yawMax,unsigned int leftBrakeMin,unsigned int rightBrakeMin);
void calibration(Joystick_ joystick,unsigned int leftBrakeMax,unsigned int rightBrakeMax,unsigned int yawMin,unsigned int yawMax,unsigned int leftBrakeMin,unsigned int rightBrakeMin);

#endif //SIM_PEDALS_CALIBRATION_H
