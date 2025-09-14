#pramga once
#include <cstdint>
#include <math.h>

class Cache {
  public:
    cache();
    int printCacheInfo();
  protected:
    uint32_t address;
    unsigned short numSets;
    unsigned short setSize;
    unsigned int lineSize;
    bool writeThrough;
    uint8_t blockOffsetSize;
    uint8_t indexSize;
    uint8_t tagSize;
};

class L1Cache : public cache {
  public:
    L1Cache(ushort numSets, ushort setSize, uint lineSize, bool writeThrough);
  private:
    void determineCacheProperties();
};