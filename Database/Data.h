#ifndef DATA
#define DATA

#include <QString>
#include "Serializible.h"
#include <vector>

using namespace std;

using namespace database;

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
    void setHeaders(vector<QString>& headers);
    vector<QString>& getHeaders();
    void setCells(vector<vector<QString> >& cells);
    vector<vector<QString> >& getCells();
    vector<QString>& getString(int num);
    bool containsString(int num);
    int addString(vector<QString>& Str);
    vector<QString>& getColumn(QString header);
    QString getCell(QString& header, int num);
    int size();
    QString serialize();
    void parse(QString json);
    struct comparator {
        bool operator() (MDPData& data1, MDPData& data2) const{
            if (data1.size() != data2.size() || data1.firstIndex != data2.firstIndex || data1.getHeaders().size() != data2.getHeaders().size()) return false;
            for (int i = 0; i < data1.getHeaders().size(); i++){
                if (data1.getHeaders()[i] != data1.getHeaders()[i]) return false;
            }
            for (int i = 0; i < data1.getCells().size(); i++){
                if (data1.getCells()[i].size() != data2.getCells()[i].size()) return false;
                for (int j = 0; j < data1.getCells()[i].size(); i++){
                    if (data1.getCells()[i][j] != data2.getCells()[i][j]) return false;
                }
            }
            return true;
        }
    };
};



#endif // DATA

