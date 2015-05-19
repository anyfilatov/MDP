#ifndef HARDDISKWORKER
#define HARDDISKWORKER

#include <QString>
#include <set>
#include <vector>
#include "HashTable/TableKey.h"

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
    QString DBSource;
    set<TableKey, TableKey::keys_comparator> tableKeys;
    void uploadTableKeys();
  protected:
    HardDiskWorker();
    ~HardDiskWorker();
    friend class HardDiskWorkDestroyer;
  public:
    static HardDiskWorker& getInstance();
    bool contains(TableKey& key);
    vector<TableKey>& keys();
};

#endif // HARDDISKWORKER

