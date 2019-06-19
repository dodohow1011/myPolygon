#include "myPolygon.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Usage: ./myPolygon [input_file] [output file]" << endl;
        return 0;
    }

    myPolygon myPol;
    string input_file = argv[1];
    string output_file = argv[2];
    if (!myPol.read_input(input_file))
        return 0;
    myPol.run();
    myPol.write(output_file);

    return 0;
}
