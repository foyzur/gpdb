subdir=src/backend/storage/ipc
top_builddir=../../../../..
include $(top_builddir)/src/Makefile.global

TARGETS=dsm

include $(top_builddir)/src/backend/mock.mk

dsm.t: \
	$(MOCK_DIR)/backend/storage/ipc/shmem_mock.o \
	$(MOCK_DIR)/backend/utils/error/assert_mock.o \
	$(MOCK_DIR)/backend/utils/error/elog_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/event_version_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/idle_tracker_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/memaccounting_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/mcxt_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/memprot_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/redzone_handler_mock.o \
	$(MOCK_DIR)/backend/utils/mmgr/runaway_cleaner_mock.o
