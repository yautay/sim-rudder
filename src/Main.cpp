
#include "Arduino.h"
#include <Joystick.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

unsigned long checkpoint;

int16_t leftBrakeMax = 0;
int16_t rightBrakeMax = 0;
int16_t yawMin = 0;
int16_t yawMax = 0;
int16_t leftBrakeMin = 0;

int16_t rightBrakeMin = 0;
int16_t channelYaw;
int16_t channelLeftBrk;

int16_t channelRightBrk;

Adafruit_ADS1115 adafruitAds1115(0x48);

Adafruit_SSD1306 display(-1);

Joystick_ joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
        0,
        0,
        false,
        false,
        false,
        true,
        true,
        true,
        false,
        false,
        false,
        false,
        false);

void readEEPROM();
void setRanges();
void calibration();
void wait(unsigned long milisecs);
void displayOledDots(int seconds);
void displayOledMinMax();
void displayOledWelcomeScr();
void displayOledDoneMsg();

void setup() {
    pinMode(9,INPUT_PULLUP);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    displayOledWelcomeScr();
    displayOledDots(5);
    adafruitAds1115.begin();
    adafruitAds1115.setGain(GAIN_ONE);
    joystick.begin();
    displayOledDoneMsg();
    wait(2000);
    readEEPROM();
    setRanges();
    displayOledMinMax();
}

void loop() {
    channelYaw = adafruitAds1115.readADC_SingleEnded(0);
    channelRightBrk = adafruitAds1115.readADC_SingleEnded(1);
    channelLeftBrk = adafruitAds1115.readADC_SingleEnded(2);

    joystick.setRzAxis(channelYaw);  // Yaw
    joystick.setRxAxis(channelLeftBrk);  // LeftBrake
    joystick.setRyAxis(channelRightBrk);  // RightBrake

    if (!digitalRead(9)) {
        calibration();
        readEEPROM();
        setRanges();
    }
}

void readEEPROM(){
    yawMin = (EEPROM.read(0) | (EEPROM.read(1) << 8));
    yawMax = (EEPROM.read(2) | (EEPROM.read(3) << 8));
    leftBrakeMin = (EEPROM.read(4) | (EEPROM.read(5) << 8));
    leftBrakeMax = (EEPROM.read(6) | (EEPROM.read(7) << 8));
    rightBrakeMin = (EEPROM.read(8) | (EEPROM.read(9) << 8));
    rightBrakeMax = (EEPROM.read(10) | (EEPROM.read(11) << 8));
}
void setRanges(){
    joystick.setRzAxisRange(yawMin, yawMax);
    joystick.setRxAxisRange(leftBrakeMin, leftBrakeMax);
    joystick.setRyAxisRange(rightBrakeMin, rightBrakeMax);
}
void calibration(){
    leftBrakeMax = INT16_MIN;
    rightBrakeMax = INT16_MIN;
    yawMin = INT16_MAX;
    yawMax = INT16_MIN;

    leftBrakeMin = adafruitAds1115.readADC_SingleEnded(2);
    rightBrakeMin = adafruitAds1115.readADC_SingleEnded(1);

    while (!digitalRead(9)) {

        uint16_t tmp;

        tmp = adafruitAds1115.readADC_SingleEnded(0);
        if (yawMin > tmp){
            yawMin = tmp;
        }

        tmp = adafruitAds1115.readADC_SingleEnded(0);
        if (yawMax < tmp){
            yawMax = tmp;
        }

        tmp = adafruitAds1115.readADC_SingleEnded(2);
        if (rightBrakeMax < tmp){
            rightBrakeMax = tmp;
        }

        tmp = adafruitAds1115.readADC_SingleEnded(1);
        if (leftBrakeMax < tmp){
            leftBrakeMax = tmp;
        }
    }

    EEPROM.write(0, lowByte(yawMin));
    EEPROM.write(1, highByte(yawMin));
    EEPROM.write(2, lowByte(yawMax));
    EEPROM.write(3, highByte(yawMax));
    EEPROM.write(4, lowByte(leftBrakeMin));
    EEPROM.write(5, highByte(leftBrakeMin));
    EEPROM.write(6, lowByte(leftBrakeMax));
    EEPROM.write(7, highByte(leftBrakeMax));
    EEPROM.write(8, lowByte(rightBrakeMin));
    EEPROM.write(9, highByte(rightBrakeMin));
    EEPROM.write(10, lowByte(rightBrakeMax));
    EEPROM.write(11, highByte(rightBrakeMax));
}
void displayOledMinMax(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("   Min/Max values: ");
    display.print("Yaw: ");
    display.print(yawMin);
    display.print(" / ");
    display.println(yawMax);
    display.print("L Brk: ");
    display.print(leftBrakeMin);
    display.print(" / ");
    display.println(leftBrakeMax);
    display.print("R brk: ");
    display.print(rightBrakeMin);
    display.print(" / ");
    display.println(rightBrakeMax);
    display.display();
}
void displayOledDots(int seconds){
    checkpoint = millis();
    do {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        for (int x = 1; x <= seconds; x++){
            display.setCursor(x * 10 , 10);
            display.println(".");
            display.display();
            wait(1000);
        }
    } while (millis() - checkpoint <= seconds * 1000);
}
void displayOledWelcomeScr(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(30, 10);
    display.println("SIM-RUDDER");
    display.setCursor(15, 20);
    display.println("INITIALIZATION...");
    display.display();

    wait(2500);
}
void displayOledDoneMsg(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(40, 10);
    display.println("DONE :)");
    display.display();
}
void wait(unsigned long milisecs){
    unsigned long tmp = millis();
    do {
    } while (millis() - tmp <= milisecs);
}