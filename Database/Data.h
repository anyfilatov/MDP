#ifndef DATA
#define DATA

#include <QString>
#include "Serializible.h"
#include <vector>

using namespace std;

class MDPData:public Serializible{
private:
    int firstIndex;
    vector<QString> headers;
    vector<vector<QString> > cells;
    int findHeaderColumn(QString header);
public:
    MDPData();
    MDPData(vector<QString> headers, vector<vector<QString> > cells, int firstIndex);
    ~MDPData();
    void setFirstIndex(int index);
    int getFirstIndex();
    void setHeaders(vector<QString> headers);
    vector<QString> getHeaders();
    void setCells(vector<vector<QString> >& cells);
    vector<vector<QString> >& getCells();
    vector<QString> getString(int num);
    bool containsString(int num);
    int addString(vector<QString> Str);
    vector<QString> getColumn(QString header);
    QString getCell(QString header, int num);
    int size();
    QString serialize();
    void parse(QString json);
    void generateRandom() {
        for(int j =0; j < 100000; j++){
            vector<QString>  v(14);
            for(int i =0; i < 14; i++){
                v[i] = QString::number(1.0);
            }
            cells.push_back(v);
        }
    }

};


#endif // DATA

