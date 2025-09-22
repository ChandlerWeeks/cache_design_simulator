#include "cache.h"

CacheLine::CacheLine() {
  this->tag = 0;
  this->dirty = false;
  this->valid = false;
  this->timestamp = 0;
}

bool CacheLine::isCacheHit(uint32_t tag) {
  return this->tag == tag;
}

bool CacheLine::isValid() {
  return this->valid;
}

bool CacheLine::replaceCache(uint32_t tag, bool dirty) {
  this->tag = tag;
  this->valid = true;
  this->dirty = dirty;
  return true;
}

void CacheLine::setTimestamp(uint64_t timestamp) {
  this->timestamp = timestamp;
}

void CacheLine::setDirtyBit() {
  this->dirty = true;
}

uint64_t CacheLine::getTimestamp() {
  return timestamp;
}

bool CacheLine::isDirty() {
  return this->dirty;
}