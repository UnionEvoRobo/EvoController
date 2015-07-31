/*
 * EvoLib.cpp - Library for running the EvoFab Silicone Extruder
 * Created by Frank Chiarulli Jr., July 2, 2015.
 */

#include "Arduino.h"
#include "EvoLib.h"

        EvoExtruder::EvoExtruder(long pauseTime, int inputPin1, int inputPin2, int releasePin1, int releasePin2){
            _pins[0] = inputPin1;
            _pins[1] = inputPin2;
            _pins[2] = releasePin1;
            _pins[3] = releasePin2;
            _INPUT_VALVE = _pins[0];
            _RELEASE_VALVE = _pins[2];
            for(int i = 0; i < 4; i++){
                pinMode(_pins[i],OUTPUT);
            }
            _pauseTime = pauseTime;
            _released = millis();
        }
        void EvoExtruder::extrude(){
            digitalWrite(_RELEASE_VALVE,LOW);
            digitalWrite(_INPUT_VALVE,HIGH);
        }
        void EvoExtruder::pause(){
            digitalWrite(_INPUT_VALVE,LOW);
            digitalWrite(_RELEASE_VALVE,HIGH);
            _released = millis();
        }
        void EvoExtruder::check(){
            unsigned long currentMillis = millis();
            if(currentMillis - _released > _pauseTime){
                digitalWrite(_RELEASE_VALVE,LOW);
            }
        }

        EvoEndstop::EvoEndstop(int pin, long debounce){
            _pin = pin;
            _debounce = debounce;
            _time = 0;
            _pressed = false;
            pinMode(_pin,INPUT);
        }
        EvoEndstop::EvoEndstop(int pin){
            _pin = pin;
            _debounce = 400;
            _time = 0;
            _pressed = false;
            pinMode(_pin,INPUT);
        }
        bool EvoEndstop::pressed(){
            if(digitalRead(_pin) == HIGH && millis() - _time > _debounce) {
                _time = millis();
                return true;
            } else {
                return false;
            }
        }
