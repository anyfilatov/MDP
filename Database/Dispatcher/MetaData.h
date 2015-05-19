#ifndef METADATA
#define METADATA

#include <QString>
#include "Serializible.h"
#include <vector>

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

};

#endif // METADATA

