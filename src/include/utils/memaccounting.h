/*-------------------------------------------------------------------------
 *
 * meminstrumentation.h
 *	  This file contains declarations for memory instrumentation utility
 *	  functions.
 *
 * Portions Copyright (c) 2013, Greenplum inc
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $PostgreSQL: pgsql/src/include/utils/meminstrumentation.h,v 1.00 2013/03/22 14:57:00 rahmaf2 Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef MEMACCOUNTING_H
#define MEMACCOUNTING_H

#include "nodes/nodes.h"
#include "lib/stringinfo.h"             /* StringInfo */

struct MemoryContextData;

/* Macros to define the level of memory accounting to show in EXPLAIN ANALYZE */
#define EXPLAIN_MEMORY_VERBOSITY_SUPPRESS  0 /* Suppress memory reporting in explain analyze */
#define EXPLAIN_MEMORY_VERBOSITY_SUMMARY  1 /* Summary of memory usage for each owner in explain analyze */
#define EXPLAIN_MEMORY_VERBOSITY_DETAIL  2 /* Detail memory accounting tree for each slice in explain analyze */

#define LEAK_DETECTION_LEVEL_DISABLE  0
#define LEAK_DETECTION_LEVEL_LAST  1
#define LEAK_DETECTION_LEVEL_ALL  2

/*
 * What level of details of the memory accounting information to show during EXPLAIN ANALYZE?
 */
extern int explain_memory_verbosity;

extern int leak_detection_level;
extern int leak_detection_ignore;

/*
 * Unique run id for memory profiling. May be just a start timestamp for a batch of queries such as TPCH
 */
extern char* memory_profiler_run_id;

/*
 * Dataset ID. Determined by the external script. One example could be, 1: TPCH, 2: TPCDS etc.
 */
extern char* memory_profiler_dataset_id;

/*
 * Which query of the query suite is running currently. E.g., query 21 of TPCH
 */
extern char* memory_profiler_query_id;

/*
 * Scale factor of TPCH/TPCDS etc.
 */
extern int memory_profiler_dataset_size;

/*
 * Should we save the memory usage information before resetting the memory accounting?
 */
extern bool gp_dump_memory_usage;

/*
 * Each memory account can assume one of the following memory
 * owner types
 */
typedef enum MemoryOwnerType
{
	/* Long-living accounts that survive reset */
	// Reserve 0 for undefined/uninitialized memory account id
	MEMORY_OWNER_TYPE_Undefined = 0,
	MEMORY_OWNER_TYPE_LogicalRoot,
	MEMORY_OWNER_TYPE_SharedChunkHeader,
	MEMORY_OWNER_TYPE_Rollover,
	MEMORY_OWNER_TYPE_MemAccount,
	MEMORY_OWNER_TYPE_END_LONG_LIVING = MEMORY_OWNER_TYPE_MemAccount,
	/* End of long-living accounts */

	/* Short-living accounts */
	MEMORY_OWNER_TYPE_Top,
	MEMORY_OWNER_TYPE_MainEntry,
	MEMORY_OWNER_TYPE_Parser,
	MEMORY_OWNER_TYPE_Planner,
	MEMORY_OWNER_TYPE_Optimizer,
	MEMORY_OWNER_TYPE_Dispatcher,
	MEMORY_OWNER_TYPE_Serializer,
	MEMORY_OWNER_TYPE_Deserializer,

	MEMORY_OWNER_TYPE_EXECUTOR_START,
	MEMORY_OWNER_TYPE_EXECUTOR = MEMORY_OWNER_TYPE_EXECUTOR_START,
	MEMORY_OWNER_TYPE_Exec_Result,
	MEMORY_OWNER_TYPE_Exec_Append,
	MEMORY_OWNER_TYPE_Exec_Sequence,
	MEMORY_OWNER_TYPE_Exec_BitmapAnd,
	MEMORY_OWNER_TYPE_Exec_BitmapOr,
	MEMORY_OWNER_TYPE_Exec_SeqScan,
	MEMORY_OWNER_TYPE_Exec_ExternalScan,
	MEMORY_OWNER_TYPE_Exec_AppendOnlyScan,
	MEMORY_OWNER_TYPE_Exec_AOCSScan,
	MEMORY_OWNER_TYPE_Exec_TableScan,
	MEMORY_OWNER_TYPE_Exec_DynamicTableScan,
	MEMORY_OWNER_TYPE_Exec_IndexScan,
	MEMORY_OWNER_TYPE_Exec_DynamicIndexScan,
	MEMORY_OWNER_TYPE_Exec_BitmapIndexScan,
	MEMORY_OWNER_TYPE_Exec_BitmapHeapScan,
	MEMORY_OWNER_TYPE_Exec_BitmapAppendOnlyScan,
	MEMORY_OWNER_TYPE_Exec_TidScan,
	MEMORY_OWNER_TYPE_Exec_SubqueryScan,
	MEMORY_OWNER_TYPE_Exec_FunctionScan,
	MEMORY_OWNER_TYPE_Exec_TableFunctionScan,
	MEMORY_OWNER_TYPE_Exec_ValuesScan,
	MEMORY_OWNER_TYPE_Exec_NestLoop,
	MEMORY_OWNER_TYPE_Exec_MergeJoin,
	MEMORY_OWNER_TYPE_Exec_HashJoin,
	MEMORY_OWNER_TYPE_Exec_Material,
	MEMORY_OWNER_TYPE_Exec_Sort,
	MEMORY_OWNER_TYPE_Exec_Agg,
	MEMORY_OWNER_TYPE_Exec_Unique,
	MEMORY_OWNER_TYPE_Exec_Hash,
	MEMORY_OWNER_TYPE_Exec_SetOp,
	MEMORY_OWNER_TYPE_Exec_Limit,
	MEMORY_OWNER_TYPE_Exec_Motion,
	MEMORY_OWNER_TYPE_Exec_ShareInputScan,
	MEMORY_OWNER_TYPE_Exec_Window,
	MEMORY_OWNER_TYPE_Exec_Repeat,
	MEMORY_OWNER_TYPE_Exec_DML,
	MEMORY_OWNER_TYPE_Exec_SplitUpdate,
	MEMORY_OWNER_TYPE_Exec_RowTrigger,
	MEMORY_OWNER_TYPE_Exec_AssertOp,
	MEMORY_OWNER_TYPE_Exec_AlienShared,
	MEMORY_OWNER_TYPE_Exec_BitmapTableScan,
	MEMORY_OWNER_TYPE_Exec_PartitionSelector,
	MEMORY_OWNER_TYPE_EXECUTOR_END = MEMORY_OWNER_TYPE_Exec_PartitionSelector,

} MemoryOwnerType;

/****
 * The following are constants to define additional memory stats
 * (in addition to memory accounts) during CSV dump of memory balance
 */
/* vmem reserved from memprot.c */
#define MEMORY_STAT_TYPE_VMEM_RESERVED -1
/* Peak memory observed from inside memory accounting among all allocations */
#define MEMORY_STAT_TYPE_MEMORY_ACCOUNTING_PEAK -2
/***************************************************************************/

struct MemoryAccount;
struct MemoryAccountArray;

extern struct MemoryAccount* ActiveMemoryAccount;
extern struct MemoryAccount* RolloverMemoryAccount;
extern struct MemoryAccount* AlienExecutorMemoryAccount;
extern struct MemoryAccount* SharedChunkHeadersMemoryAccount;

extern uint64 MemoryAccountingOutstandingBalance;
extern uint64 MemoryAccountingPeakBalance;

extern uint16 MemoryAccountingCurrentGeneration;

typedef uint64 MemoryAccountIdType;

// Array of accounts available
extern struct MemoryAccountArray* memoryAccountArray;

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
	// array of pointers to memory accounts of size accountCount
	MemoryAccount** allAccounts;
} MemoryAccountArray;

/*
 * Instead of pointers to construct the tree, the SerializedMemoryAccount
 * uses "serial" of each node and saves parent serial to construct the tree.
 * This is required as we cannot serialize pointers. As an optimization, we
 * can later on try to reuse the pointers themselves and treat them as integer
 * to save the "serial"
 */
typedef struct SerializedMemoryAccount {
	NodeTag type;
	MemoryAccount memoryAccount;

	/*
	 * memoryAccountSerial and parentMemoryAccountSerial are used for serializing
	 * MemoryAccount. Note: we cannot serialize the tree using the pointers.
	 * Instead we serialize these "serial" and "parent serial" and construct the
	 * tree at the destination (e.g., dispatcher or any reporting tool).
	 */
	uint32 memoryAccountSerial;
	/* If memoryAccountSerial == parentMemoryAccountSerial, then the node has NO parent */
	uint32 parentMemoryAccountSerial;
} SerializedMemoryAccount;

/*
 * START_MEMORY_ACCOUNT would switch to the specified newMemoryAccount,
 * saving the oldActiveMemoryAccount. Must be paired with END_MEMORY_ACCOUNT
 */
#define START_MEMORY_ACCOUNT(newMemoryAccountId)  \
	do { \
		MemoryAccount *oldActiveMemoryAccount = NULL; \
		Assert(NULL != memoryAccountArray); \
		Assert(newMemoryAccountId < memoryAccountArray->accountCount); \
		oldActiveMemoryAccount = ActiveMemoryAccount; \
		ActiveMemoryAccount = memoryAccountArray[newMemoryAccountId];\
/*
 * END_MEMORY_ACCOUNT would restore the previous memory account that was
 * active at the time of START_MEMORY_ACCCOUNT call
 */
#define END_MEMORY_ACCOUNT()  \
		ActiveMemoryAccount = oldActiveMemoryAccount;\
	} while (0);

/*
 * CREATE_EXECUTOR_MEMORY_ACCOUNT is a convenience macro to create a new
 * operator specific memory account *if* the operator will be executed in
 * the current slice, i.e., it is not part of some other slice (alien
 * plan node). We assign a shared AlienExecutorMemoryAccount for plan nodes
 * that will not be executed in current slice
 */
#define CREATE_EXECUTOR_MEMORY_ACCOUNT(isAlienPlanNode, planNode, NodeType) \
		(MEMORY_OWNER_TYPE_Undefined != planNode->memoryAccountId) ?\
			planNode->memoryAccountId : \
			(isAlienPlanNode ? AlienExecutorMemoryAccount->id : \
				MemoryAccounting_CreateAccount(((Plan*)node)->operatorMemKB == 0 ? \
				work_mem : ((Plan*)node)->operatorMemKB, MEMORY_OWNER_TYPE_Exec_##NodeType));

/*
 * SAVE_EXECUTOR_MEMORY_ACCOUNT saves an operator specific memory account
 * into the PlanState of that operator
 */
#define SAVE_EXECUTOR_MEMORY_ACCOUNT(execState, curMemoryAccountId)\
		Assert(MEMORY_OWNER_TYPE_Undefined == ((PlanState *)execState)->plan->memoryAccountId || \
		MEMORY_OWNER_TYPE_Undefined == ((PlanState *)execState)->plan->memoryAccount || \
		curMemoryAccountId == ((PlanState *)execState)->plan->memoryAccountId);\
		((PlanState *)execState)->plan->memoryAccountId = curMemoryAccountId;

extern MemoryAccountIdType
MemoryAccounting_CreateAccount(long maxLimit, enum MemoryOwnerType ownerType);

extern MemoryAccountIdType
MemoryAccounting_SwitchAccount(MemoryAccountIdType desiredAccountId);

extern void
MemoryAccounting_Reset(void);

extern uint32
MemoryAccounting_Serialize(StringInfoData* buffer);

extern SerializedMemoryAccount*
MemoryAccounting_Deserialize(const void *serializedBits,
		uint32 memoryAccountCount);

extern uint64
MemoryAccounting_GetPeak(MemoryAccount *memoryAccount);

extern uint64
MemoryAccounting_GetBalance(MemoryAccount *memoryAccount);

extern void
MemoryAccounting_ToString(MemoryAccount *root, StringInfoData *str,
		uint32 indentation);

extern void
MemoryAccounting_SaveToFile(int currentSliceId);

extern uint32
MemoryAccounting_SaveToLog(void);

extern const char*
MemoryAccounting_GetAccountName(MemoryAccountIdType memoryAccountId);

extern void
MemoryAccounting_ToCSV(MemoryAccount *root, StringInfoData *str, char *prefix);

extern void
MemoryAccounting_PrettyPrint(void);
/*
 * MemoryAccountIsValid
 *		True iff memory account is valid.
 */
#define MemoryAccountIsValid(memoryAccount) \
	((memoryAccount) != NULL && \
	 ( IsA((memoryAccount), MemoryAccount) ))

#endif   /* MEMACCOUNTING_H */
