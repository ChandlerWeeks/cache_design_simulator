#include "DC.h"

DataCache::DataCache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, bool useL2) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->lineSize = lineSize;
  this->writeThrough = writeThrough;
  this->useL2 = useL2;
  this->l2Cache = nullptr;
  this->timestamp = 0;
  determineCacheProperties();
  initializeCacheStructure();
}

void DataCache::determineCacheProperties() {
  cacheSize = numSets * setSize * lineSize;
  blockOffsetSize = log2(lineSize);
  indexSize = log2(numSets);
  tagSize = 32 - indexSize - blockOffsetSize;
}

// oftentimes L2 is initialized after DC so add a setter
void DataCache::setL2Cache(L2Cache* l2Cache) {
  if (useL2) {
    this->l2Cache = l2Cache;
  } else {
    this->l2Cache = nullptr;
  }
}

void DataCache::printCacheProperties() {
  std::cout << "D-cache contains " << numSets << " sets.\n";
  std::cout << "Each set contains " << setSize << " entries.\n";
  std::cout << "Each line is " << lineSize << " bytes.\n";
  if (!writeThrough) {
    std::cout << "The Cache uses a write-allocate and write-back policy\n";
  } else {
    std::cout << "The Cache uses a no-write-allocate and write-through policy\n";
  }
  std::cout << "Number of bits used for the index is " << indexSize << ".\n";
  std::cout << "Number of bits used for the offset is " << blockOffsetSize << ".\n";
}

void DataCache::cacheRead(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res) {
  uint32_t offset;
  uint32_t index;
  uint32_t tag;

  extract_address_properties(address, &offset, &index, &tag);
  indexOut = index;
  tagOut = tag;

  std::vector<CacheLine>& set = cache[index];
  // check each block, I hate using lines/blocks interchangeably but I did
  for (CacheLine &line : set) {
    if (line.isCacheHit(tag)) {
      // cache hit
      line.setTimestamp(timestamp++);
      stats->incrementCacheHits("DC");
      res = 1;
      return;
    }
  }

  // cache miss
  stats->incrementCacheMisses("DC");
  res = 0;

  if (!useL2) {
    // no L2, go to main memory
    stats->incrementMainMemoryAccesses();
  }

  // select victim: select LRU
  CacheLine* victim = &set[0];
  for (CacheLine &line : set) {
    if (line.getTimestamp() < victim->getTimestamp()) {
      victim = &line;
    }
  }

  // write back dirty victim to next level
  if (victim->isDirty() && victim->isValid()) {
    if (useL2) {
      uint32_t wbIdx, wbTag; short wbRes;
      //l2Cache->cacheWrite(address, wbIdx, wbTag, wbRes);
    } else {
      stats->incrementMainMemoryAccesses();
    }
  }

  // install fetched block
  victim->replaceCache(tag, false);
  victim->setTimestamp(timestamp++);
}

void DataCache::invalidateFromPFN(uint32_t PFN) {
  // invalidate all cache lines belonging to the PFN
  uint32_t pageSizeBytes = 1u << (blockOffsetSize + indexSize); // assuming DC is at least page sized
  uint32_t dcIndex, dcTag, dcOffset;
  uint32_t basePhys = PFN << (blockOffsetSize + indexSize);

  extract_address_properties(basePhys, &dcOffset, &dcIndex, &dcTag);

  std::cout << "invalidating DC line with tag " << dcTag << " and index " << dcIndex << " since phys page " << PFN << " is being replaced\n";

  invalidateAddress(basePhys); // only check the first offset ig
}

void DataCache::invalidateAddress(uint32_t address) {
  uint32_t offset;
  uint32_t index;
  uint32_t tag;

  extract_address_properties(address, &offset, &index, &tag);

  std::vector<CacheLine>& set = cache[index];
  for (CacheLine &line : set) {
    if (line.isCacheHit(tag)) {
      line.invalidate();
      if (useL2) {
        l2Cache->invalidateAddress(address);
      }
      return;
    }
  }
}

void DataCache::initializeCacheStructure() {
  cache.resize(numSets);
  for (uint32_t i = 0; i < numSets; i++) {
    cache[i].resize(setSize);
  }
}

void DataCache::extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag) {
  *offset = address & ( (1 << blockOffsetSize) - 1);
  *index = (address >> blockOffsetSize) & ( (1 << indexSize) - 1);
  *tag = address >> (blockOffsetSize + indexSize);
}