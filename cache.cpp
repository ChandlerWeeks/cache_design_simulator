#include "cache.h"

// TODO: ALWAYS TO NO WRITE ALLOCATE

Cache::Cache(uint32_t numSets, uint32_t setSize, uint32_t lineSize, bool writeThrough, std::string type) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->lineSize = lineSize;
  this->writeThrough = writeThrough;
  this->timestamp = 0;
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

bool Cache::cacheRead(uint32_t address) {
    // cache breakdown
    uint32_t offset;
    uint32_t index;
    uint32_t tag;

    extract_address_properties(address, &offset, &index, &tag);

    timestamp++;

    // find the cache set, check each block in the set
    std::vector<CacheLine>& set = cache[index];
    for (CacheLine &line : set) {
      if (line.isCacheHit(tag) && line.isValid()) {
        line.setTimestamp(timestamp);
        return true; // successful run
      }
    }

    // handle the cache miss
    // Find the cahce block to evict using LRU or whatever the fuck im supposed to do
    CacheLine* oldest = &set[0];
    for (CacheLine &line : set) {
      // just put it in an invalid line if we find one
      if (!line.isValid()) {
        oldest = &line;
        break;
      }
      if (line.getTimestamp() < oldest->getTimestamp()) {
        oldest = &line;
      }
    }

    // handle a dirty replacement
    if (oldest->isDirty() && oldest->isValid()) {
      invalidateParents(address);
      if (nextLevel != nullptr) {
        nextLevel->cacheWrite(address);
      }
    }
    
    if (nextLevel != nullptr) {
      nextLevel->cacheRead(address);
    }

    oldest->replaceCache(tag, false);
    oldest->setTimestamp(timestamp);
    return true;
}

bool Cache::cacheWrite(uint32_t address) {
    // cache breakdown
    uint32_t offset;
    uint32_t index;
    uint32_t tag;

    extract_address_properties(address, &offset, &index, &tag);

    timestamp++;
    
    // find the cache set, check each block in the set
    std::vector<CacheLine>& set = cache[index];
    for (CacheLine &line : set) {
      if (line.isCacheHit(tag) && line.isValid()) {
        // WRITE THROUGH: write down to each tier of cache
        line.setTimestamp(timestamp);
        if (writeThrough) {
          if (nextLevel != nullptr) {
            nextLevel->cacheWrite(address);
          }
          return true; // successful run
        } 
        // Write Back policy: dont write down to each tier, dirty bit
        else { 
          line.setDirtyBit();
          return true; // successful run
        }
      }
    }

    // TODO: For whatever godforsaken reason, i didnt read shit right, fix this
    // ONLY DO NO WRITE ALLOCATE
    // write through no write allocate
    if (writeThrough) {
      return true; // just write to memory
    }
    // write back write allocate, fetch memory into cache, then perform the cache write
    else {
      cacheRead(address); // fetch the address from memory
      // get the cache block
      for (CacheLine &line : set) {
        if (line.isValid() && line.isCacheHit(tag)) {
          line.setTimestamp(timestamp);
          if (nextLevel != nullptr) {
            nextLevel->cacheWrite(address);
          }
          break;
        }
      }

    }
    return true;
}

// extract the address from cache and invalidate it
bool Cache::invalidateAddress(uint32_t address) {
  uint32_t offset;
  uint32_t index;
  uint32_t tag;

  extract_address_properties(address, &offset, &index, &tag);

  std::cout << "RAHHH INVALIDING THIS BIT" << std::endl;

  std::vector<CacheLine>& set = cache[index];
    for (CacheLine &line : set) {
      if (line.isCacheHit(tag)) {
        line.invalidate();
        return true; // successful run
      }
    }
  return false; // did not successfully invalidate the address
}

// TODO: this needs to ensure inclusivity, there should be a block in l1 than points to this one, if it changes invalidate the **others**
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