#ifndef JsonMethods_h
#define JsonMethods_h

#include <string>
#include <vector>

using namespace std;

class JsonMethods {
private:
        static string trim(string str);
        static string clean(string str);
public:
        static vector<pair<string, string> > parseJson(string json);
        static string toJsonString(vector<pair<string, string> > object);
        static short int getInt(string number);
};

#endif

