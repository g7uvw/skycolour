// Just some time functions I tend to reuse

#include "timestuff.h"

void delay_ms(unsigned int delay_ms)
{
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = delay_ms * 1000000; //10 ms
  nanosleep(&tim , &tim2);
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
