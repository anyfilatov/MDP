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
//    friend QDataStream& operator << (QDataStream& stream, const MDPData& d){
//        stream << d.firstIndex ;
//        stream << d.headers.size();
//        for(auto& s : d.headers){
//            stream << s;
//        }
//        stream << d.cells.size();
//        for(auto& cell : d.cells){
//            stream << cell.size();
//            for(auto& s : cell) {
//                stream << s;
//            }
//        }
//        return stream;
//    }
//      friend QDataStream& operator >> (QDataStream& stream, MDPData& d){
//          int hSize = 0;
//          stream >> d.firstIndex >> hSize;
//          d.headers.resize(hSize);
//          for(int i =0; i < hSize;i++){
//              stream >> d.headers[i];
//          }
//          int cSize = 0;
//          stream >> cSize;
//          d.cells.resize(cSize);
//          for(int i =0; i < cSize; i ++){
//              int inSize = 0;
//              stream >> inSize;
//              std::vector<QString> vec(inSize);
//              for(int j = 0; j < inSize; j++){
//                    stream >> vec[j];
//              }
//              d.cells[i].swap(vec);
//          }

//      }
    void generateRandom(int count) {
        for(int j =0; j < count; j++){
            vector<QString>  v(14);
            for(int i =0; i < 14; i++){
                v[i] = QString::number(1.0);
            }
            cells.push_back(v);
        }
    }

};


#endif // DATA

