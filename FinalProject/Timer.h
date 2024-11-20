#pragma once
class Timer {
public:
    Timer();
    enum STATE { READY, CHASE, SCATTER, RESPONSE, FRIGHTENED, TRANSITION, NON_WORKING, BLACKSHOWN};
    void update(int deltaTime);
    void setState(STATE s);
    bool checkchange(STATE a, int t);

    int getelapsedTime() const;
    STATE getState() const;
    void initialize(STATE s, int t);

private:
    int elapsedTime;
    STATE state;
};


class GameTimer : public Timer {
public:

    GameTimer();
    void setReadyInitialized(bool s);
    
    int getreadyTime() const;
    int getchaseTime() const;
    int getscatterTime() const;
    int getresponseTime() const;
    bool getReadyInitialized() const;


private:
    int readyTime;
    int chaseTime;
    int scatterTime;
    int responseTime;
    bool ReadyInitialized;
};

class FrightenedTimer : public Timer {
public:
    FrightenedTimer();
    int getfrightenedTime() const;
    int gettransitionTime() const;
    int getblinkingInterval() const;

private:
    int frightenedTime;
    int transitionTime;
    int blinkingInterval;
};

class BlackshownTimer : public Timer {
public:
    BlackshownTimer();
    int getblackshownTime() const;

private:
    int blackshownTime;
};


