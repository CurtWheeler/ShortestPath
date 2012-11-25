#include <iostream>
#include "gtest.h"
#include "MemoryList.hpp"

MemoryList memoryList;

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
   
  RUN_ALL_TESTS();
  
  memoryList.checkAllocationStatus();
  std::cin.get();

  return 0;
}

