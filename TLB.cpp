#include "TLB.h"

TLB::TLB(uint32_t numSets, uint32_t setSize) {
  this->numSets = numSets;
  this->setSize = setSize;
  this->indexBits = log2(numSets);
}

void TLB::printTLBInfo() {
  std::cout << "Data TLB contains " << this->numSets << " sets.\n";
  std::cout << "Each set contains " << this->setSize << " entries.\n";
  std::cout << "Number of bits used for the index is " << this->indexBits << ".\n";
}