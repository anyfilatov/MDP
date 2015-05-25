#ifndef HARDDISKWORKER
#define HARDDISKWORKER

#include <QString>
#include <vector>
#include "HashTable/HashTable.h"
#include "HashTable/TableKey.h"
#include "Dispatcher/MetaData.h"
#include "Data.h"

using namespace std;

class HardDiskWorker;

class HardDiskWorkDestroyer
{
  private:
    HardDiskWorker* p_instance;
  public:
    HardDiskWorkDestroyer() {}
    ~HardDiskWorkDestroyer();
    void initialize(HardDiskWorker* worker);
};

class HardDiskWorker
{
  private:
    static HardDiskWorker* p_instance;
    static HardDiskWorkDestroyer destroyer;
    int maxFileSize;
    QString DBSource;
    HashTable<TableKey, MetaData> meta;
    void uploadMetaData();
    void updateMetaFile(short int userId, short int dataId, short int processId, MetaData* metaData);
    void putCells(short int userId, short int dataId, short int processId, int oldSize, const vector<vector<QString> > &cells);
  protected:
    friend class HardDiskWorkDestroyer;
  public:
    HardDiskWorker();
    ~HardDiskWorker();
    static HardDiskWorker& getInstance();
    bool contains(TableKey& key);
    int getSize(TableKey& key);
    const vector<QString>& getHeaders(TableKey& key);
    int getMaxFileSize();
    vector<TableKey>& keys();
    void put(short int userId, short int dataId, short int processId, MDPData* data);
    vector<vector<QString> > get(short int userId, short int dataId, short int processId, int part);
};

#endif // HARDDISKWORKER

