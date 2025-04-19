#include "Airport.h"

/*
Constructor
Sets up the airport with its name/code and state code
*/
Airport::Airport(const std::string& air_code, const std::string& state_code){
  this->air_code = air_code;
  this->state_code = state_code;
}

// compares aiports based on their name/code
bool Airport::operator==(const Airport& other) const {
  return air_code == other.air_code;
}
