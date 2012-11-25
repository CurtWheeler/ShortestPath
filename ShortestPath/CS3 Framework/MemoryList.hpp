#ifndef _MEMORY_POOL_HPP
#define _MEMORY_POOL_HPP

/**
 * @file MemoryList.hpp
 * @addtogroup Utilities
 *
 * @brief Header file for the MemoryList class.
 *
 * Revision History:
 * Dec 08, 2010  B. Hansen      / SDL  Creation
 * Sep 26, 2011  M. Greenman    / SDL  Improvements #416, #555, Updates to Logger to make calls more convenient, and more performant.
 * Oct 11, 2011  J. Christensen / SDL  Moved function definition to remove forward declaration
 * Nov 14, 2011  J. Christensen / SDL  Integrated MEMORY_PROFILING
 * Mar 20, 2012  B. Hansen      / SDL  Added MemoryList::PRE_ALLOC_BYTES constant to make the number of pre-allocation bytes configurable
 */

#include <stdlib.h> //Includes NULL
#include <stdint.h>
#include <string>
#include <cstdio>
#include <list>

#define UInt8   uint8_t
#define Int8    int8_t
#define UInt16  uint16_t
#define Int16   int16_t
#define UInt32  uint32_t
#define Int32   int32_t
#define UInt64  uint64_t
#define Int64   int64_t

#define Float32 float
#define Float64 double



/**
 * @class BlockInfo
 *
 * @brief Data container class used to store information and state of a block in the memory pool.
 */
class AllocationInfo
{
  public:
    AllocationInfo(void);
	  ~AllocationInfo(void);

    void* m_pAllocation;         //!< Pointer to start of block in pool
    UInt32 m_allocSize;          //!< Size of total allocation block pertains to
    UInt32 m_line;
    std::string m_file;
};

/**
 * @class MemoryList
 *
 * @brief Class used to control memory allocation through use of a
 * pre-allocated pool.
 */
class MemoryList
{
  public:
    MemoryList();
    ~MemoryList();

    void* allocate(UInt32 bytes, char* file, UInt32 line);
    void deallocate(AllocationInfo* pLocation);
            
    void checkAllocationStatus(void);
	

    /**
     * @brief Class constants
     */
    enum
    {
      PRE_ALLOC_BYTES = 8  //Number of bytes reserverd before each allocation for system use and to fulfill byte alignment requirements
    };
    
  private:
	  std::list<AllocationInfo*> m_allocationInformation; //!< List containing information for every allocated block     
    UInt32 m_numAllocations;
    UInt32 m_numDeallocations;
    bool m_tracking;

    MemoryList(const MemoryList&);              //!< Private and undefined to prevent usage
    MemoryList& operator= (const MemoryList&);  //!< Private and undefined to prevent usage
};


//***********Overloaded new and delete operators***************//

#define nouveau new(__FILE__, __LINE__, false)
#define neuf new(__FILE__, __LINE__, true)

void* operator new(size_t numBytes) throw(std::bad_alloc);
void* operator new(size_t numBytes, char* file, UInt32 line, bool track) throw(std::bad_alloc);
void operator delete(void* pAllocatedMemory) throw();

void* operator new[](size_t numBytes) throw(std::bad_alloc);
void operator delete[](void* pAllocatedMemory) throw();

void* operator new(size_t numBytes, const std::nothrow_t& rNt) throw();
void operator delete(void* pAllocatedMemory, const std::nothrow_t& rNt) throw();

void* operator new[](size_t numBytes, const std::nothrow_t& rNt) throw();
void operator delete[](void* pAllocatedMemory, const std::nothrow_t& rNt) throw();


#endif // _MEMORY_POOL_HPP
