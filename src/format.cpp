#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours = 0;
    int minutes = 0;
    string seconds_string = "";
    string minutes_string = "";
    string hours_string = "";

    minutes = seconds/60; 
    hours = minutes/60; 
    minutes = minutes%60; // to get remainder of minutes
    seconds = seconds%60; // same for seconds

    // to always have 2 digits
    if (hours < 10) hours_string = "0" + to_string(hours);
    else hours_string = to_string(hours);
    if (minutes < 10) minutes_string = "0" + to_string(minutes);
    else minutes_string = to_string(minutes);
    if (seconds < 10) seconds_string = "0" + to_string(seconds);
    else seconds_string = to_string(seconds);

    return hours_string + ":" + minutes_string + ":" + seconds_string; 
}