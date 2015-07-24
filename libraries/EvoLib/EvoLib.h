/*
 * EvoLib.h - Library for running the EvoFab Silicone Extruder
 * Created by Frank Chiarulli Jr., July 2, 2015.
 */

#ifndef EvoLib_h
#define EvoLib_h

class EvoExtruder
{
    public:
        EvoExtruder(long pauseTime, int inputPin1, int inputPin2, int releasePin1, int releasePin2);
        void extrude();
        void pause();
        void check();
    private:
        long _pauseTime;
        long _released;
        int _pins[4];
        int _INPUT_VALVE;
        int _RELEASE_VALVE;
};

class EvoEndstop
{
    public:
        EvoEndstop(int pin, long debounce);
        EvoEndstop(int pin);
        void check();
        bool pressed();
    private:
        long _pin;
        long _debounce;
        long _time;
        bool _pressed;
};

#endif
