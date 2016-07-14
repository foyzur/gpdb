/*-------------------------------------------------------------------------
 *
 * memaccounting_private.h
 *	  This file contains declarations for memory accounting functions that
 *	  are only supposed to be used by privileged callers such a memory managers.
 *	  Other files should not include this file.
 *
 * Copyright (c) 2016, Pivotal Inc.
 *
 *-------------------------------------------------------------------------
 */
#ifndef MEMACCOUNTING_PRIVATE_H
#define MEMACCOUNTING_PRIVATE_H

#include "utils/memaccounting.h"

extern MemoryAccountIdType liveAccountStartId;
extern MemoryAccountIdType nextAccountId;

/* MemoryAccount is the fundamental data structure to record memory usage */
typedef struct MemoryAccount {
	NodeTag type;
	MemoryOwnerType ownerType;

	uint64 allocated;
	uint64 freed;
	uint64 peak;
	/*
	 * Maximum targeted allocation for an owner. Peak usage can be tracked to
	 * check if the allocation is overshooting
	 */
	uint64 maxLimit;

	MemoryAccountIdType id;
	MemoryAccountIdType parentId;
} MemoryAccount;

typedef struct MemoryAccountArray{
	MemoryAccountIdType accountCount;
	MemoryAccountIdType arraySize;
	// array of pointers to memory accounts of size accountCount
	MemoryAccount** allAccounts;
} MemoryAccountArray;

extern MemoryAccountArray* shortLivingMemoryAccountArray;
extern MemoryAccount* longLivingMemoryAccountArray[MEMORY_OWNER_TYPE_END_LONG_LIVING + 1];

extern MemoryAccount *SharedChunkHeadersMemoryAccount;

extern uint64 MemoryAccountingOutstandingBalance;
extern uint64 MemoryAccountingPeakBalance;

static inline bool
MemoryAccounting_IsValidAccount(MemoryAccountIdType id)
{
	AssertImply(NULL == shortLivingMemoryAccountArray, liveAccountStartId == nextAccountId);
	return (id < liveAccountStartId + (NULL == shortLivingMemoryAccountArray ? 0 : shortLivingMemoryAccountArray->accountCount));
}

static inline MemoryAccount*
MemoryAccounting_ConvertIdToAccount(MemoryAccountIdType id)
{
	MemoryAccount *memoryAccount = NULL;

	if (id >= liveAccountStartId)
	{
		Assert(NULL != shortLivingMemoryAccountArray);
		Assert(id < liveAccountStartId + shortLivingMemoryAccountArray->accountCount);
		memoryAccount = shortLivingMemoryAccountArray->allAccounts[id - liveAccountStartId];
	}
	else if (id <= MEMORY_OWNER_TYPE_END_LONG_LIVING)
	{
		Assert(NULL != longLivingMemoryAccountArray);
		/* 0 is reserved as undefined. So, the array index is 1 behind */
		memoryAccount = longLivingMemoryAccountArray[id];
	}
	else if (id < liveAccountStartId)
	{
		Assert(NULL != longLivingMemoryAccountArray);
		memoryAccount = longLivingMemoryAccountArray[MEMORY_OWNER_TYPE_Rollover];
	}

	Assert(IsA(memoryAccount, MemoryAccount));

	return memoryAccount;
}

/*
 * MemoryAccounting_Allocate
 *	 	When an allocation is made, this function will be called by the
 *	 	underlying allocator to record allocation request.
 *
 * memoryAccountId: where to record this allocation
 * allocatedSize: the final amount of memory returned by the allocator (with overhead)
 *
 * If the return value is false, the underlying memory allocator should fail.
 */
static inline bool
MemoryAccounting_Allocate(MemoryAccountIdType memoryAccountId, Size allocatedSize)
{
	Assert(MemoryAccounting_IsValidAccount(memoryAccountId));
	MemoryAccount* memoryAccount = MemoryAccounting_ConvertIdToAccount(memoryAccountId);

	Assert(memoryAccount->allocated + allocatedSize >=
			memoryAccount->allocated);

	memoryAccount->allocated += allocatedSize;

	Size held = memoryAccount->allocated -
			memoryAccount->freed;

	memoryAccount->peak =
			Max(memoryAccount->peak, held);

	Assert(memoryAccount->allocated >=
			memoryAccount->freed);

	MemoryAccountingOutstandingBalance += allocatedSize;
	MemoryAccountingPeakBalance = Max(MemoryAccountingPeakBalance, MemoryAccountingOutstandingBalance);

	return true;
}

/*
 * MemoryAccounting_Free
 *		"One" implementation of free request handler. Each memory account
 *		can customize its free request function. When memory is deallocated,
 *		this function will be called by the underlying allocator to record deallocation.
 *		This function records the amount of memory freed.
 *
 * memoryAccount: where to record this allocation
 * context: the context where this memory belongs
 * allocatedSize: the final amount of memory returned by the allocator (with overhead)
 *
 * Note: the memoryAccount can be an invalid pointer if the generation of
 * the allocation is different than the current generation. In such case
 * this method would automatically select RolloverMemoryAccount, instead
 * of accessing an invalid pointer.
 */
static inline bool
MemoryAccounting_Free(MemoryAccountIdType memoryAccountId, Size allocatedSize)
{
	MemoryAccount* memoryAccount = MemoryAccounting_ConvertIdToAccount(memoryAccountId);

	Assert(memoryAccount->freed +
			allocatedSize >= memoryAccount->freed);

	Assert(memoryAccount->allocated >= memoryAccount->freed);

	memoryAccount->freed += allocatedSize;

	MemoryAccountingOutstandingBalance -= allocatedSize;

	Assert(MemoryAccountingOutstandingBalance >= 0);

	return true;
}

#endif   /* MEMACCOUNTING_PRIVATE_H */
