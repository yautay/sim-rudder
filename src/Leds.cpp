//
// Created by yauta on 26.03.2020.
//
#include <Arduino.h>
#include "Leds.h"


void ledSong(int led1, int led2, int led3){
    digitalWrite(led1,HIGH);
    delay(1500);
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
    delay(1500);
    digitalWrite(led2,LOW);
    digitalWrite(led3,HIGH);
    delay(1500);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    delay(1000);
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    delay(500);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    delay(1000);
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    delay(500);
    digitalWrite(led1,HIGH);
    digitalWrite(led2,HIGH);
    digitalWrite(led3,HIGH);
    delay(1000);
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
}
