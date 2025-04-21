#include "read_cvs.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

vector<vector<string>> read_cvs(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }
    string line;
    file.ignore(numeric_limits<streamsize>::max(), '\n');  // Skip header row

    while (getline(file, line)) { //takes an entire line from the cvs to process it.
        stringstream aa(line);
        string a;
        string origin, destination, O_City, O_State, D_City, D_State, distance, cost;
        int index = 0;

        while (getline(aa, a, ',')) {
            // Clean string (remove quotes and spaces)
            a.erase(remove(a.begin(), a.end(), '"'), a.end());
            a.erase(remove(a.begin(), a.end(), ' '), a.end());

            // Assign strings to variables;
            switch (index) {
                case 0: origin = a; break;
                case 1: destination = a; break;
                case 2: O_City = a; break;
                case 3: O_State = a; break;
                case 4: D_City = a; break;
                case 5: D_State = a; break;
                case 6: distance = a; break;
                case 7: cost = a; break;
                default: break;
            }
            index++;  // Increment per loop
        }

        // Store  values into data vector
        vector<string> row = {origin, destination, O_City, O_State, D_City, D_State, distance, cost};
        data.push_back(row);

        // Print the parsed row
      //  cout << origin << "  " << destination << "  " << O_City << "  " << O_State
      //       << "  " << D_City << "  " << D_State << "  " << distance << "  " << cost << endl;
    }

    file.close();
    return data;
}

