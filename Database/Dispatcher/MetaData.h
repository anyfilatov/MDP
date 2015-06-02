#ifndef METADATA
#define METADATA

#include <QString>
#include "Serializible.h"
#include <vector>

namespace database{

class MetaData:public Serializible{
private:
    int size;
    vector<QString> headers;
public:
    MetaData();
    MetaData(vector<QString> headers, int size);
    ~MetaData();
    void setSize(int size);
    int getSize();
    void setHeaders(vector<QString>& headers);
    vector<QString>& getHeaders();
    QString serialize();
    void parse(QString json);
    struct comparator {
        bool operator() (MetaData& data1, MetaData& data2) const{
            if (data1.getSize() != data2.getSize() || data1.getHeaders().size() != data2.getHeaders().size()) return false;
            for (int i = 0; i < data1.getHeaders().size(); i++){
                if (data1.getHeaders()[i] != data1.getHeaders()[i]) return false;
            }
            return true;
        }
    };
    struct cloneFunc {
        MetaData* operator() (MetaData& data){
            return new MetaData(data.getHeaders(), data.getSize());
        }
    };
};

}

#endif // METADATA

