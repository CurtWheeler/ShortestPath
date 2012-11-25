/**
 * @file MemoryList.cpp
 * @addtogroup Utilities
 *
 * @brief Implementation File for the MemoryList class.
 *
 * Revision History:
 * Dec 08, 2010  B. Hansen      / SDL  Initial Implementation
 * Sep 26, 2011  M. Greenman    / SDL  Improvements #416, #555, Updates to Logger to make calls more convenient, and more performant.
 * Oct 11, 2011  J. Christensen / SDL  Removed logging statements because they cause infinite recursion with MemoryListController
 * Oct 20, 2011  B. Hansen      / SDL  Failed pool allocations now results in termination of the calling process
 * Nov 14, 2011  J. Christensen / SDL  Integrated MEMORY_PROFILING
 * Jan 19, 2012  I. Karlinsey   / SDL  Added variable initialization in delete operator
 * Mar 20, 2012  B. Hansen      / SDL  Now MemoryList::PRE_ALLOC_BYTES are allocated prior to each allocation to store the MemoryList* and any padding needed
 *                                       for proper memory alignment
 */

#include <cstdio>
#include "gtest.h"
#include "MemoryList.hpp"

/**
 * @brief MemoryList class constructor
 */
MemoryList::MemoryList()
  : m_numAllocations(0),
    m_numDeallocations(0),
    m_tracking(false)
{
  // empty
}

/**
 * @brief MemoryList class destructor, deletes heap allocated memory
 */
MemoryList::~MemoryList()
{  
  // empty

}

/**
 * @brief Allocate memory from this pool
 *
 * @param bytes Size in bytes to allocate from this pool. This may exceed the block size if contiguous blocks are available in the pool.
 *
 * @return Pointer to the allocated memory or NULL if allocation failed.
 *
 * @note PRE_ALLOC_BYTES extra bytes are actually allocated under the hood to allow for more manageable deletes.
 */
void* MemoryList::allocate(UInt32 bytes, char* file, UInt32 line)
{  
  ++m_numAllocations;

  void* pAllocation = NULL;
  AllocationInfo* pInfo = new AllocationInfo(); 
  
  pAllocation = malloc(bytes + MemoryList::PRE_ALLOC_BYTES);     
  *(AllocationInfo**)pAllocation = pInfo; // put a pointer to the AllocationInfo in the front of the allocation
  pAllocation = (UInt8*)pAllocation + MemoryList::PRE_ALLOC_BYTES; // Don't let the user see the extra allocation

  pInfo->m_pAllocation = pAllocation;  
  pInfo->m_allocSize = bytes;  
  pInfo->m_line = line;
  
  std::string fullFileName (file);

  size_t location = fullFileName.find_last_of("\\");  
  pInfo->m_file = fullFileName.substr(location + 1);    
  m_allocationInformation.push_back(pInfo);
            
  return pAllocation;    
}

/**
 * @brief Deallocate memory from this pool.
 *
 * @param pLocation Pointer to the memory to deallocate.
 */
void MemoryList::deallocate(AllocationInfo* pInfo)
{  
  ++m_numDeallocations;  
  m_allocationInformation.remove(pInfo);
}

void MemoryList::checkAllocationStatus(void)
{
  EXPECT_EQ(0, m_allocationInformation.size());
  if (m_allocationInformation.size() > 0)
  {
    printf("Leaked %i allocation(s)\n\n\n", m_allocationInformation.size());
  
    UInt32 count = 0;  

    for (std::list<AllocationInfo*>::iterator it = m_allocationInformation.begin(); it != m_allocationInformation.end(); it++ )
    {
      ++count;
   
      printf("Leak #%i\n", count);
      printf("-------\n");
      printf("File: %s\n", (*it)->m_file.c_str());
      printf("Line: %i\n", (*it)->m_line);
      printf("Address: 0x%p\n", (*it)->m_pAllocation);
      printf("Total bytes leaked: %i\n", (*it)->m_allocSize);
      printf("\n\n");   
    }
  }
}

/**
 * @brief BlockInfo default constructor.
 */
AllocationInfo::AllocationInfo(void)
  : m_pAllocation(NULL),
    m_allocSize(0)
{
  // empty
}

AllocationInfo::~AllocationInfo(void)
{
  // empty
}


extern MemoryList memoryList;

//***********Overloaded new and delete operators***************//


void* operator new(size_t numBytes) throw(std::bad_alloc)
{
  void* pRetVal = NULL;
  void* pAllocation = NULL;

  if(numBytes > 0)
  {
    pAllocation = malloc(numBytes + MemoryList::PRE_ALLOC_BYTES); // overallocate by bytes needed for MemoryPool*
    if(pAllocation == NULL)
    {
      throw std::bad_alloc();
    }
    *(AllocationInfo**)pAllocation = NULL;                    // use NULL in the global new
    pRetVal = (UInt8*)pAllocation + MemoryList::PRE_ALLOC_BYTES;  // don't let users see the MemoryPool*
  }

  return pRetVal;
}

/**
 * @brief Overloading of the global new operator.
 *
 * @param numBytes Number of bytes to allocate.
 *
 * @return Pointer to allocated memory
 *
 * @note This overloading allocates the size of a pointer extra memory at the front
 * of the block (transparently to user) to allow for things to play nicely with the
 * memory pool and the globally overloaded delete.
 *
 * @throws std::bad_alloc Use new(std::nothrow) to avoid exceptions
 */
void* operator new(size_t numBytes, char* file, UInt32 line, bool track) throw(std::bad_alloc)
{
  void* pRetVal = NULL;  

  if(numBytes > 0)
  {
    pRetVal = memoryList.allocate(numBytes, file, line);  // over allocate by bytes needed for MemoryPool* and memory alignment

    if (track)
    {
      printf("Just allocated: 0x%p\n", pRetVal);
    }
  }

  return pRetVal;
}

/**
 * @brief Overloading of the global delete operator.
 *
 * @param pAllocatedMemory Pointer to allocated memory block.
 *
 * @note When this delete is called for something allocated from a MemoryList it will call the Pools dealloc
 * function, if it was allocated from the heap free() is called.
 */
void operator delete(void* pAllocatedMemory) throw()
{  
  AllocationInfo* pInfo = NULL;

  if(pAllocatedMemory != NULL)
  {
    pAllocatedMemory = (UInt8*)pAllocatedMemory - MemoryList::PRE_ALLOC_BYTES;  // Include hidden MemoryPool*
    pInfo = *(AllocationInfo**)pAllocatedMemory;

    if(pInfo == NULL) //Global delete operator
    {
      free(pAllocatedMemory);  //Deletes entire memory chunk including the extra MemoryPool::PRE_ALLOC_BYTES bytes
    }
    else  //Delete associated with a memory pool
    {
      memoryList.deallocate(pInfo);  //Deletes entire memory chunk including the extra MemoryPool::PRE_ALLOC_BYTES bytes
      free(pAllocatedMemory);  //Deletes entire memory chunk including the extra MemoryPool::PRE_ALLOC_BYTES bytes
    }
  }
}

/**
 * @brief Overloading of the global new[] operator.
 *
 * @param numBytes Number of bytes to allocate.
 *
 * @return Pointer to allocated memory or NULL on failure.
 */
void* operator new[](size_t numBytes) throw(std::bad_alloc)
{
  return operator new(numBytes);
}

/**
 * @brief Overloading of the global delete[] operator.
 *
 * @param pAllocatedMemory Pointer to allocated memory block.
 */
void operator delete[](void* pAllocatedMemory) throw()
{
  operator delete(pAllocatedMemory);
}

/**
 * @brief Overloading of the global new(nothrow) operator.
 *
 * @param numBytes Number of bytes to allocate.
 * @param rNt nothrow parameter
 *
 * @return Pointer to allocated memory or NULL on failure.
 *
 * @note This overloading allocates the size of a pointer extra memory at the front
 * of the block (transparently to user) to allow for things to play nicely with the
 * memory pool and the globally overloaded delete.
 */
void* operator new(size_t numBytes, const std::nothrow_t& rNt) throw()
{
  void* pRetVal = NULL;
  void* pAllocation = NULL;

  if(numBytes > 0)
  {
    pAllocation = malloc(numBytes + MemoryList::PRE_ALLOC_BYTES); // overallocate by bytes needed for MemoryList*
    if(pAllocation != NULL)
    {
      *(MemoryList**)pAllocation = NULL;                    // use NULL in the global new
      pRetVal = (UInt8*)pAllocation + MemoryList::PRE_ALLOC_BYTES;  // don't let users see the MemoryList*
    }
    else
    {
      printf("Allocation of %lu bytes failed\n", static_cast<unsigned long>(numBytes));
      fflush(NULL);
      exit(-1);
    }
  }

  return pRetVal;
}

/**
 * @brief Overloading of the global delete(nothrow) operator.
 *
 * @param pAllocatedMemory Pointer to allocated memory block.
 * @param rNt nothrow parameter
 */
void operator delete(void* pAllocatedMemory, const std::nothrow_t& rNt) throw()
{
  operator delete(pAllocatedMemory);
}

/**
 * @brief Overloading of the global new[](nothrow) operator.
 *
 * @param numBytes Number of bytes to allocate.
 * @param rNt nothrow parameter
 *
 * @return Pointer to allocated memory or NULL on failure.
 */
void* operator new[](size_t numBytes, const std::nothrow_t& rNt) throw()
{
  return operator new(numBytes, rNt);
}

/**
 * @brief Overloading of the global delete[](nothrow) operator.
 *
 * @param pAllocatedMemory Pointer to allocated memory block.
 * @param rNt nothrow parameter
 */
void operator delete[](void* pAllocatedMemory, const std::nothrow_t& rNt) throw()
{
  operator delete(pAllocatedMemory);
}

