#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmockery.h"

#include "../dsm.c"

#define EXPECT_EXCEPTION()     \
	expect_any(ExceptionalCondition,conditionName); \
	expect_any(ExceptionalCondition,errorType); \
	expect_any(ExceptionalCondition,fileName); \
	expect_any(ExceptionalCondition,lineNumber); \
    will_be_called_with_sideeffect(ExceptionalCondition, &_ExceptionalCondition, NULL);\

#define EXPECT_ELOG(LOG_LEVEL)     \
    will_be_called(elog_start); \
	expect_any(elog_start, filename); \
	expect_any(elog_start, lineno); \
	expect_any(elog_start, funcname); \
	if (LOG_LEVEL < ERROR) \
	{ \
    	will_be_called(elog_finish); \
	} \
    else \
    { \
    	will_be_called_with_sideeffect(elog_finish, &_ExceptionalCondition, NULL);\
    } \
	expect_value(elog_finish, elevel, LOG_LEVEL); \
	expect_any(elog_finish, fmt); \

#define EXPECT_EREPORT(LOG_LEVEL)     \
	expect_any(errstart, elevel); \
	expect_any(errstart, filename); \
	expect_any(errstart, lineno); \
	expect_any(errstart, funcname); \
	expect_any(errstart, domain); \
	if (LOG_LEVEL < ERROR) \
	{ \
    	will_return(errstart, false); \
	} \
    else \
    { \
    	will_return_with_sideeffect(errstart, false, &_ExceptionalCondition, NULL);\
    } \

#define SEGMENT_VMEM_CHUNKS_TEST_VALUE 100

#define PG_RE_THROW() siglongjmp(*PG_exception_stack, 1)

/*
 * This method will emulate the real ExceptionalCondition
 * function by re-throwing the exception, essentially falling
 * back to the next available PG_CATCH();
 */
void
_ExceptionalCondition()
{
     PG_RE_THROW();
}

void InitSharedMemory()
{
	dynamic_shared_memory_type = DSM_IMPL_POSIX;
	assert_true(dsm_control == NULL);

	EXPECT_ELOG(DEBUG2);
	EXPECT_ELOG(DEBUG2);

	dsm_postmaster_startup();
	assert_true(dsm_control != NULL);
}

/*
 * This method sets up necessary data structures.
 */
void DSMTestSetup(void **state)
{
	InitSharedMemory();
}

/*
 * This method cleans up.
 */
void DSMTestTeardown(void **state)
{
}
/*
 * Checks if we can allocate from dsm
 */
void
test__DSM_Allocate__CanAllocate(void **state)
{
}

int
main(int argc, char* argv[])
{
	cmockery_parse_arguments(argc, argv);

	const UnitTest tests[] = {
		unit_test_setup_teardown(test__DSM_Allocate__CanAllocate, DSMTestSetup, DSMTestTeardown),
	};

	return run_tests(tests);
}
