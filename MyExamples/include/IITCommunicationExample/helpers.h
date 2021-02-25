
#include <stdlib.h>
#include <chrono> 
#include <ctime>

using namespace std::chrono;

double millis()
{   
 //high_resolution_clock::time_point t = high_resolution_clock::now();
 //long long t_ms = duration_cast<milliseconds>(t.time_since_epoch()).count();
//high_resolution_clock::time_point tp = system_clock::now();
 //high_resolution_clock::duration dtn = tp.time_since_epoch();
 //long long durMs = timePointMs - epochStartMs;
 //long long t_ms = dtn.count() * high_resolution_clock::period::num /high_resolution_clock::period::den;
 unsigned long t_ms = 
    std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();


 return  (double)t_ms;
}