#include <iostream>
#include <string>
#include <sstream>
using namespace std;


int main()
{

    string line;

    //use stringstream
    getline(cin,line,'\n');
    stringstream ss(line);
    cout << line << endl;
    string s;
    ss >> s;
    ss.seekp(8);
    ss >> s;
    cout << s << endl;

    return EXIT_SUCCESS;
}