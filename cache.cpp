#include "cache.h"

Cache::Cache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, std::string type) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->lineSize = lineSize;
  this->writeThrough = writeThrough;
  this->timestamp = 0;
  this->cacheType = type;
  this->nextLevel = nullptr;
  this->parentCache = nullptr;
  determineCacheProperties();
  initializeCacheStructure();
}

uint32_t Cache::getLineSize() {
  return lineSize;
}

// set the cache below this cache (dc -> l2 -> main memory)
void Cache::setNextLevel(Cache* nextLevel) {
  this->nextLevel = nextLevel;
}

void Cache::setParent(Cache* parent) {
  this->parentCache = parent;
}

void Cache::determineCacheProperties() {
  cacheSize = numSets * setSize * lineSize;
  blockOffsetSize = log2(lineSize);
  indexSize = log2(numSets);
  tagSize = 32 - indexSize - blockOffsetSize;
}

void Cache::printCacheProperties() {
  std::cout << cacheType << "Cache contains " << numSets << " sets.\n";
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

// initializes the cache with invalid bits
void Cache::initializeCacheStructure() {
  cache.resize(numSets); // dimension 1

  // set each block size;
  for (uint32_t i=0; i < numSets; ++i) {
    cache[i].resize(setSize);
  }
}

void Cache::extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag) {
  uint32_t offsetMask = (1 << blockOffsetSize) - 1;
  uint32_t indexMask  = (1 << indexSize) - 1; 

  *offset = address & offsetMask;
  *index = (address >> blockOffsetSize) & indexMask;
  *tag = address >> (blockOffsetSize + indexSize);
}

void Cache::cacheRead(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res) {
  // cache breakdown
  uint32_t offset;
  uint32_t index;
  uint32_t tag;

  extract_address_properties(address, &offset, &index, &tag);
  indexOut = index;
  tagOut = tag;
  // find the cache set, check each block in the set
  std::vector<CacheLine>& set = cache[index];
  for (CacheLine &line : set) {
    if (line.isCacheHit(tag) && line.isValid()) {
      line.setTimestamp(timestamp++);
      stats->incrementCacheHits(cacheType);
      res = true;
      return;
    }
  }
  // handle the cache miss
  // Find the cache block to evict using LRU
  res = false;
  if (nextLevel == nullptr) {
    stats->incrementMainMemoryAccesses();
  }
  stats->incrementCacheMisses(cacheType);
  CacheLine* oldest = &set[0];
  for (CacheLine &line : set) {
    if (line.getTimestamp() < oldest->getTimestamp()) {
      oldest = &line;
    }
  }

  // handle a dirty replacement
  if (oldest->isDirty() && oldest->isValid()) {
    invalidateParents(address);
    if (nextLevel != nullptr) {
      uint32_t dummy1, dummy2;
      short dummy3;
      nextLevel->cacheWrite(address, dummy1, dummy2, dummy3);
    }
  }
  oldest->replaceCache(tag, false);
  oldest->setTimestamp(timestamp);
}

void Cache::cacheWrite(uint32_t address, uint32_t &indexOut, uint32_t &tagOut, short &res) {
  // cache breakdown
  uint32_t offset;
  uint32_t index;
  uint32_t tag;

  extract_address_properties(address, &offset, &index, &tag);
  indexOut = index;
  tagOut = tag;
  
  // find the cache set, check each block in the set
  std::vector<CacheLine>& set = cache[index];
  for (CacheLine &line : set) {
    if (line.isCacheHit(tag) && line.isValid()) {
      stats->incrementCacheHits(cacheType);
      res = true;
      // WRITE THROUGH: write down to each tier of cache
      line.setTimestamp(timestamp++);
      return;
      // Write Back policy: dont write down to each tier, dirty bit
      if (!writeThrough) { 
        line.setDirtyBit();
        return;
      }
    }
  }
  
  // write through no write allocate
  res = false;
  stats->incrementCacheMisses(cacheType);
  if (writeThrough) {
    // just write to memory??
    stats->incrementMainMemoryAccesses();
  }
  // write back write allocate, fetch memory into cache, then perform the cache write
  else {
    // write-back + write-allocate on write miss
    stats->incrementMainMemoryAccesses();

    // select victim: prefer invalid, else LRU
    CacheLine* victim = &set[0];
    for (CacheLine &line : set) {
      if (!line.isValid()) {
        victim = &line;
        break;
      } else if (line.getTimestamp() < victim->getTimestamp()) {
        victim = &line;
      }
    }

    // write back dirty victim to next level
    if (victim->isDirty() && victim->isValid()) {
      invalidateParents(address);
      if (nextLevel) {
        uint32_t wbIdx, wbTag; short wbRes;
        nextLevel->cacheWrite(address, wbIdx, wbTag, wbRes);
      }
    }

    // install fetched block and mark dirty for this write
    victim->replaceCache(tag, true);
    victim->setTimestamp(timestamp++);
  }
}

// extract the address from cache and invalidate it
void Cache::invalidateAddress(uint32_t address) {
  uint32_t offset;
  uint32_t index;
  uint32_t tag;

  extract_address_properties(address, &offset, &index, &tag);

  std::vector<CacheLine>& set = cache[index];
  for (CacheLine &line : set) {
    if (line.isCacheHit(tag)) {
      line.invalidate();
      if (parentCache != nullptr) {
        parentCache->invalidateAddress(address);
      }
    }
  }
}

void Cache::invalidateParents(uint32_t address) {
  if (parentCache == nullptr) {
    return;
  } if (lineSize == parentCache->getLineSize()) {
    parentCache->invalidateAddress(address);
    return;
  }

  // Parent has smaller blocks. We need to send multiple invalidations.
  uint32_t parentLineSize = parentCache->getLineSize();
  uint32_t blockRatio = this->lineSize / parentLineSize;

  // Calculate the base address of the current cache's block
  uint32_t blockStartMask = ~((1 << this->blockOffsetSize) - 1);
  uint32_t baseAddress = address & blockStartMask;

  // Loop and invalidate each corresponding parent block
  for (uint32_t i = 0; i < blockRatio; ++i) {
    uint32_t addrToInvalidate = baseAddress + (i * parentLineSize);
    parentCache->invalidateAddress(addrToInvalidate);
  }
}