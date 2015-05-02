#ifndef JsonMethods_cpp
#define JsonMethods_cpp

#include "JsonMethods.h"
#include <string>
#include <vector>
#include <conio.h>
#include <fstream>
#include <QTextStream>

using namespace std;

string JsonMethods::trim(string str) {
    if(str.empty())
            return str;

    std::size_t first = str.find_first_not_of(' ');
    std::size_t last  = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}

string JsonMethods::clean(string str) {
    if(str.empty())
        return str;
    if ((str[0] == '"') && (str[str.length() - 1] == '"')){
        str = str.substr(1, str.length() - 2);
    }
    return str;
}

vector<pair<string, string> > JsonMethods::parseJson(string json){
    QTextStream cout(stdout);
    vector<pair<string, string> > Object = *(new vector<pair<string, string> >());
    json = trim(json);
    if ((json[0] != '{') || (json[json.length() - 1] != '}')) {
        return Object;
    }
    string key = "", value = "";
    int position = 1, nestedObjs = 0;
    bool getKey = false, getValue = false;
    while (position < (json.length() - 1)){
        //cout << "position: " << position << ", char: " << json[position] << ", nested: " << nestedObjs;
        //cout << ", key: " << QString::fromStdString(key) << ", value: " << QString::fromStdString(value) << endl;
        if (nestedObjs > 0){
            if (getKey)
                key += json[position];
            else if (getValue)
                value += json[position];
        }else if (json[position] != '"'){
            if (getKey){
                //cout << "NOT \" AND GETKEY: raise key: " << QString::fromStdString(key);
                key += json[position];
                //cout << ", new: " << QString::fromStdString(key) << endl;
            }else if (getValue){
                //cout << "NOT \" AND GETVALUE: raise value: " << QString::fromStdString(value);
                value += json[position];
                //cout << ", new: " << QString::fromStdString(value) << endl;
            }
        }else if ((json[position] == '"') && (nestedObjs <= 0)){
            //cout << "\" ";
            if (getKey){
                if (json[position + 1] == ':'){
                    //cout << "AND GETKEY\n";
                    getKey = false;
                    key += json[position];
                }else{
                    key += json[position];
                }
            }else if (getValue){
                if ((json[position + 1] == ',') || (json[position + 1] == '}')){
                    //cout << "AND GETVALUE: PUSH, CLEAN KEY AND VALUE\n";
                    value += json[position];
                    getValue = false;
                    Object.push_back(pair<string, string>(JsonMethods::clean(key),JsonMethods::clean(value)));
                    key = "";
                    value = "";
                    nestedObjs = 0;
                }else{
                    value += json[position];
                }
            }else{
                //cout << "NOT GETKEY AND NOT GETVALUE\n";
                if ((key == "") && ((json[position - 1] == ',') || (json[position - 1] == '{'))){
                    //cout << "START NEW KEY: ";
                    getKey = true;
                    key += json[position];
                    //cout << QString::fromStdString(key) << endl;
                }else if ((value == "") && (json[position - 1] == ':')){
                    //cout << "START NEW VALUE: ";
                    getValue = true;
                    nestedObjs = -1;
                    value += json[position];
                    //cout << QString::fromStdString(value) << endl;
                }
            }
        }
        if ((json[position] == '{') && (nestedObjs != -1) && ((position == 0) || (json[position - 1] == '"') || (json[position - 1] == ':'))){
            nestedObjs++;
            if (!getKey){
                getValue = true;
                if  (nestedObjs == 1)
                    value += "{";
            }
        }else if ((json[position] == '}') && (nestedObjs != -1) && ((position == json.length() - 2) || (json[position + 1] == '"') || (json[position + 1] == ',') || (json[position + 1] == '}'))){
            nestedObjs--;
            if ((nestedObjs == 0) && (!getKey)){
                getValue = false;
                Object.push_back(pair<string, string>(JsonMethods::clean(key),JsonMethods::clean(value)));
                key = "";
                value = "";
                nestedObjs = 0;
            }
        }
        position++;
    }
    return Object;
}

string JsonMethods::toJsonString(vector<pair<string, string> > object){
    string json = "{";
    for (int i = 0; i < object.size(); i++){
        string key = object[i].first;
        string value = object[i].second;
        if ((key[0] != '"') || (key[key.size() - 1] != '"')) key = "\"" + key + "\"";
        if ((value[0] != '{') && (value[value.size() - 1] != '}')){
            if ((!value[0] != '"') || (value[value.size() - 1] != '"') || (value.length() == 1)) value = "\"" + value + "\"";
        }
        if (i != 0)
            json += ",";
        json += key + ":" + value ;
    }
    json += "}";
    return json;
}

short int JsonMethods::getInt(string number){
    int val = 0;
    for (int i = number.size() - 1, k = 1; i >= 0; i--, k *= 10) {
        val += (number[i] - 48) * k;
    }
    return val;
}

#endif
