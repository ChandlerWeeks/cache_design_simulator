#include "TLB.h"

TLBEntry::TLBEntry(uint32_t tag, uint32_t index, bool valid, uint32_t PFN, uint32_t timestamp) {
  this->tag = tag;
  this->index = index;
  this->valid = valid;
  this->PFN = PFN;
  this->timestamp = timestamp;
}

uint32_t TLBEntry::getTag() {
  return tag;
}

void TLBEntry::setTag(uint32_t tag) {
  this->tag = tag;
}

uint32_t TLBEntry::getIndex() {
  return index;
}

void TLBEntry::setIndex(uint32_t index) {
  this->index = index;
}

bool TLBEntry::isValid() {
  return valid;
}

void TLBEntry::setValid(bool valid) {
  this->valid = valid;
}

uint32_t TLBEntry::getPFN() {
  return PFN;
}

void TLBEntry::setPFN(uint32_t PFN) {
  this->PFN = PFN;
}

void TLBEntry::setTimestamp(uint32_t timestamp) {
  this->timestamp = timestamp;
}

uint32_t TLBEntry::getTimestamp() {
  return timestamp;
}