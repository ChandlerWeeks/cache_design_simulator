#include "pageTable.h"

PageTableEntry::PageTableEntry(uint32_t index, uint32_t timestamp, bool valid, bool dirty) {
  this->index = index;
  this->timestamp = timestamp;
  this->valid = valid;
  this->dirty = dirty;
}

uint32_t PageTableEntry::getIndex() {
  return index;
}

void PageTableEntry::setIndex(uint32_t index) {
  this->index = index;
}

uint32_t PageTableEntry::getTimestamp() {
  return timestamp;
}

void PageTableEntry::setTimestamp(uint32_t timestamp) {
  this->timestamp = timestamp;
}

bool PageTableEntry::isValid() {
  return valid;
}

void PageTableEntry::setValid(bool valid) {
  this->valid = valid;
}

bool PageTableEntry::isDirty() {
  return dirty;
}

void PageTableEntry::setDirty(bool dirty) {
  this->dirty = dirty;
}
