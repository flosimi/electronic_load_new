// SIMULATION STUB - software stopwatch based on millis()
#ifndef TIMER_STUB_H
#define TIMER_STUB_H
#include <Arduino.h>

class MCP79410_Timer {
  public:
    MCP79410_Timer(byte){}
    MCP79410_Timer(){}
    void start(){ if(!running){ running = true; startedAt = millis(); } }
    void stop(){ if(running){ accumulated += millis() - startedAt; running = false; } }
    void reset(){ accumulated = 0; startedAt = millis(); }
    uint32_t getTotalSeconds(){ return (accumulated + (running ? millis() - startedAt : 0)) / 1000; }
    String getTime(){ return String(getTotalSeconds()); }
  private:
    bool running = false;
    uint32_t startedAt = 0, accumulated = 0;
};
#endif
