#include "cache.h"

Cache::Cache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, std::string type) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->lineSize = lineSize;
  this->writeThrough = writeThrough;
  determineCacheProperties();
  initializeCacheStructure();
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
  if (writeThrough) {
    std::cout << "The Cache uses a write-through and write no-write allocate policy\n";
  } else {
    std::cout << "The Cache uses a write-back and write allocate policy\n";
  }
  std::cout << "Number of bits used for the index is " << indexSize << ".\n";
  std::cout << "Number of bits used for the offset is " << blockOffsetSize << ".\n";
}

// initializes the cache with invalid bits
bool Cache::initializeCacheStructure() {
  cache.resize(numSets); // dimension 1

  // set each block size;
  for (int i=0; i < numSets; ++i) {
    cache[i].resize(setSize);
  }
  return 1;
}

void Cache::extract_address_properties(uint32_t address, uint32_t* offset, uint32_t* index, uint32_t* tag) {
  uint32_t offsetMask = (1 << blockOffsetSize) - 1;
  uint32_t indexMask  = (1 << indexSize) - 1; 

  *offset = address & offsetMask;
  *index = (address >> blockOffsetSize) & indexMask;
  *tag = address >> (blockOffsetSize + indexSize);
}

bool Cache::cacheRead(uint32_t address, Cache *nextLevel) {
    // cache breakdown
    uint32_t offset;
    uint32_t index;
    uint32_t tag;

    extract_address_properties(address, &offset, &index, &tag);

    std::cout << "offset: " << offset
              << " index: " << index
              << " tag: "   << tag
              << std::endl;

    timestamp++;

    // find the cache set, check each block in the set
    std::vector<CacheLine>& set = cache[index];
    for (CacheLine &line : set) {
      if (line.isCacheHit(tag) && line.isValid()) {
        std::cout << "HIT" << std::endl;
        line.setTimestamp(timestamp);
        return true; // successful run
      }
    }

    std::cout << "MISS" << std::endl;
    // handle the cache miss
    CacheLine* oldest = &set[0];
    for (CacheLine line : set) {
      if (line.getTimestamp() < oldest->getTimestamp()) {
        std::cout << "oldest found" << std::endl;
        oldest = &line;
      }
    }
    // handle a dirty replacement
    if (oldest->isDirty()) {
      if (nextLevel != nullptr) {
        nextLevel->cacheWrite(address, nullptr);
      }
    }
    // TODO: Write down
    oldest->replaceCache(tag, false);
    oldest->setTimestamp(timestamp);
    return true;
}

bool Cache::cacheWrite(uint32_t address, Cache *nextLevel) {
    // cache breakdown
    uint32_t offset;
    uint32_t index;
    uint32_t tag;

    extract_address_properties(address, &offset, &index, &tag);
    
    // find the cache set, check each block in the set
    std::vector<CacheLine>& set = cache[index];
    for (CacheLine &line : set) {
      if (line.isCacheHit(tag) && line.isValid()) {
        std::cout << "HIT" << std::endl;
        // WRITE THROUGH: write down to each tier of cache
        if (writeThrough) {
          std::cout << "HIT" << std::endl;
          line.setTimestamp(timestamp);
          return true; // successful run
        } else { // Write Back policy: dont write down to each tier, dirty bit
          std::cout << "HIT" << std::endl;
          line.setTimestamp(timestamp);
          line.setDirtyBit();
          return true; // successful run
        }
      }
    }

    std::cout << "MISS" << std::endl;
    if (writeThrough) {
      return true; // just write to memory not cache on a no write allocate miss
    }
    // write down on a cache miss
    CacheLine* oldest = &set[0];
    for (CacheLine line : set) {
      if (oldest->getTimestamp() > line.getTimestamp()) {
        // allocate a new block in cache, work down
        std::cout << "" << std::endl;
        oldest = &line;
      }
    }
        // handle a dirty replacement
    if (oldest->isDirty()) {
      if (nextLevel != nullptr) {
        nextLevel->cacheWrite(address, nullptr);
      }
    }
    //TODO: Write down
    oldest->replaceCache(tag, false);
    oldest->setTimestamp(timestamp);
    return true;
}