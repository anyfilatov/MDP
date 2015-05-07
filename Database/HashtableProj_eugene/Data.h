#ifndef DATA
#define DATA

#include <QString>
#include "Serializible.h"
#include <vector>

using namespace std;

class Data:public Serializible{
private:
    int firstIndex;
    vector<QString> headers;
    vector<vector<QString> > cells;
    int findHeaderColumn(QString header);
public:
    Data();
    Data(vector<QString> headers, vector<vector<QString> > cells, int firstIndex);
    ~Data();
    void setFirstIndex(int index);
    int getFirstIndex();
    void setHeaders(vector<QString> headers);
    vector<QString> getHeaders();
    void setCells(vector<vector<QString> > cells);
    vector<vector<QString> > getCells();
    vector<QString> getString(int num);
    int addString(vector<QString> Str);
    vector<QString> getColumn(QString header);
    QString getCell(QString header, int num);
    int size();
    QString serialize();
    void parse(QString json);

};


#endif // DATA

