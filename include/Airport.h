#ifndef AIRPORT_H
#define AIRPORT_H

#include <vector>
#include <string>

class Airport{
    public:
    std::string air_code; // variable for airport code
    std::string state_code;// variable for state code

    Airport(const std::string& air_code, const std::string& state_code);
    bool operator ==(const Airport& other) const;  
};

#endif