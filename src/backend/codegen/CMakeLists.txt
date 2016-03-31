#   Copyright 2015-2016 Pivotal Software, Inc.
#
#   CMakeLists.txt
#       Cmake configuration for building GPDB codegen module.
#

cmake_minimum_required(VERSION 2.8.12)

project(gpcodegen CXX)
set(CMAKE_BUILD_FILES_DIRECTORY build)
set(CMAKE_BUILD_DIRECTORY build)

# Options. Turn on with 'cmake -Dvar_name=ON'
option(build_examples "Build examples also" OFF)

# Look for flags to enable C++11 support.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11 -fno-strict-aliasing -fwrapv -D_REENTRANT -D_THREAD_SAFE -D_POSIX_PTHREAD_SEMANTICS" COMPILER_HAS_STD_CXX11)
if (COMPILER_HAS_STD_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_HAS_STD_CXX0X)
  if (COMPILER_HAS_STD_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endif()

# Turn on all warnings.
CHECK_CXX_COMPILER_FLAG("-Wall" COMPILER_HAS_WALL)
if (COMPILER_HAS_WALL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
CHECK_CXX_COMPILER_FLAG("-pedantic" COMPILER_HAS_PEDANTIC)
if (COMPILER_HAS_PEDANTIC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

# Suppress warnings about C99 extensions that should be supported in C++11 mode.
CHECK_CXX_COMPILER_FLAG("-Wno-c99-extensions" COMPILER_HAS_WNO_C99_EXTENSIONS)
if (COMPILER_HAS_WNO_C99_EXTENSIONS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c99-extensions")
endif()

# Turn on the CODEGEN_DEBUG flag if this is a debug build.
if (CMAKE_MAJOR_VERSION GREATER 2)
  cmake_policy(SET CMP0043 NEW)
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:CODEGEN_DEBUG>
  )
else()
  set_property(
    DIRECTORY
    APPEND PROPERTY COMPILE_DEFINITIONS_DEBUG CODEGEN_DEBUG
  )
endif()

# Check for POSIX I/O syscalls needed by TemporaryFile.
include(CheckCXXSymbolExists)
CHECK_CXX_SYMBOL_EXISTS(mkstemp "stdlib.h" HAVE_POSIX_MKSTEMP)
CHECK_CXX_SYMBOL_EXISTS(write "unistd.h" HAVE_POSIX_WRITE)
CHECK_CXX_SYMBOL_EXISTS(fsync "unistd.h" HAVE_POSIX_FSYNC)
if (HAVE_POSIX_MKSTEMP AND HAVE_POSIX_WRITE AND HAVE_POSIX_FSYNC)
  set(codegen_tmpfile_sources utils/temporary_file.cc)
  set_property(DIRECTORY
      APPEND PROPERTY COMPILE_DEFINITIONS CODEGEN_HAVE_TEMPORARY_FILE)
else()
  message(WARNING "Missing required POSIX I/O syscalls for temporary files. "
                  "Line-by-line DEBUG information for generated code will not "
                  "be available.")
endif()

# Include our include paths.
include_directories(../../include)
include_directories(include)

# Pull in LLVM libraries.
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Disable RTTI (C++ run-time type information) if LLVM was built without it.
if (NOT LLVM_ENABLE_RTTI)
  CHECK_CXX_COMPILER_FLAG("-fno-rtti" COMPILER_HAS_FNO_RTTI)
  if (COMPILER_HAS_FNO_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  else()
    message(WARNING "LLVM was built without RTTI (run-time type information) "
                    "support, but compiler does not support -fno-rtti flag to "
                    "also build gpcodegen without RTTI support. You may see "
                    "linking errors about undefined references to typeinfo for "
                    "various LLVM classes.")
  endif()
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Some distros (Fedora, maybe others?) package LLVM as a single monolithic
# library instead of a shared library.
option(MONOLITHIC_LLVM_LIBRARY
       "Look for a single monolithic LLVM library instead of modular libraries"
       OFF)
if (MONOLITHIC_LLVM_LIBRARY)
  find_package(LLVMMonolithic REQUIRED)
endif()

# Pull in Clang libraries using our custom CMake module.
find_package(Clang REQUIRED)
include_directories(${CLANG_INCLUDE_DIRS})

# Core codegen library.
add_library(gpcodegen SHARED
            utils/clang_compiler.cc
            utils/codegen_utils.cc
            ${codegen_tmpfile_sources})
if(APPLE)
  set(WL_START_GROUP "")
  set(WL_END_GROUP "")
else()
  set(WL_START_GROUP "-Wl,--start-group")
  set(WL_END_GROUP "-Wl,--end-group")
endif()

target_link_libraries(gpcodegen ${WL_START_GROUP} ${CLANG_LIBRARIES} ${WL_END_GROUP})
if (MONOLITHIC_LLVM_LIBRARY)
  target_link_libraries(gpcodegen ${LLVM_MONOLITHIC_LIBRARIES})
else()
  # Here we link against the LLVM libraries that we use directly, as well as
  # those that are needed by the Clang libraries that we use (e.g. objcarcopts,
  # which the Clang frontend requires even though we do not compile any
  # objective-C). The llvm_map_components_to_libnames() function also takes care
  # of pulling in any transitive linking dependencies for the libraries we
  # specify.
  llvm_map_components_to_libnames(codegen_llvm_libs
                                  analysis bitwriter core executionengine ipo
                                  irreader linker mc mcjit native objcarcopts
                                  option passes support target)
  target_link_libraries(gpcodegen ${WL_START_GROUP} ${codegen_llvm_libs} ${WL_END_GROUP})
endif()

get_filename_component(full_install_name_dir "${CMAKE_INSTALL_PREFIX}/lib" ABSOLUTE)
set_target_properties(
    gpcodegen PROPERTIES
    INSTALL_NAME_DIR ${full_install_name_dir}
    MACOSX_RPATH ON)

# Integrate with GPDB build system.
# Here we compile all the GPDB code generators, and link them incrementally
# with the gpcodegen shared library to create a binary SUBSYS.o as expected by
# GPDB make system. We invoke the linker with -nostdlib since we don't really
# want to create a full executable.
add_executable(SUBSYS.o codegen_interface.cc codegen_manager.cc codegen_wrapper.cc slot_deform_tuple_codegen.cc)
set_target_properties(SUBSYS.o
    PROPERTIES
    LINK_FLAGS "-Wl,-r -nostdlib")
target_link_libraries(
    gpcodegen
)

add_library(hack SHARED codegen_interface.cc codegen_manager.cc codegen_wrapper.cc slot_deform_tuple_codegen.cc
../../../src/test/unit/mock/main_mock.o
../access/aocs/aocs_compaction.o
../access/aocs/aocsam.o
../access/aocs/aocssegfiles.o
../access/appendonly/aomd.o
../access/appendonly/aosegfiles.o
../access/appendonly/appendonly_compaction.o
../access/appendonly/appendonly_visimap.o
../access/appendonly/appendonly_visimap_entry.o
../access/appendonly/appendonly_visimap_store.o
../access/appendonly/appendonly_visimap_udf.o
../access/appendonly/appendonlyam.o
../access/appendonly/appendonlyblockdirectory.o
../access/appendonly/appendonlytid.o
../access/appendonly/appendonlywriter.o
../access/bitmap/bitmap.o
../access/bitmap/bitmapattutil.o
../access/bitmap/bitmapinsert.o
../access/bitmap/bitmappages.o
../access/bitmap/bitmapsearch.o
../access/bitmap/bitmaputil.o
../access/bitmap/bitmapxlog.o
../access/common/heaptuple.o
../access/common/indextuple.o
../access/common/memtuple.o
../access/common/printtup.o
../access/common/reloptions.o
../access/common/scankey.o
../access/common/tupdesc.o
../access/external/fileam.o
../access/external/url.o
../access/gin/ginarrayproc.o
../access/gin/ginbtree.o
../access/gin/ginbulk.o
../access/gin/gindatapage.o
../access/gin/ginentrypage.o
../access/gin/ginget.o
../access/gin/gininsert.o
../access/gin/ginscan.o
../access/gin/ginutil.o
../access/gin/ginvacuum.o
../access/gin/ginxlog.o
../access/gist/gist.o
../access/gist/gistget.o
../access/gist/gistproc.o
../access/gist/gistscan.o
../access/gist/gistsplit.o
../access/gist/gistutil.o
../access/gist/gistvacuum.o
../access/gist/gistxlog.o
../access/hash/hash.o
../access/hash/hashfunc.o
../access/hash/hashinsert.o
../access/hash/hashovfl.o
../access/hash/hashpage.o
../access/hash/hashscan.o
../access/hash/hashsearch.o
../access/hash/hashutil.o
../access/heap/heapam.o
../access/heap/hio.o
../access/heap/tuptoaster.o
../access/index/genam.o
../access/index/indexam.o
../access/nbtree/nbtcompare.o
../access/nbtree/nbtinsert.o
../access/nbtree/nbtpage.o
../access/nbtree/nbtree.o
../access/nbtree/nbtsearch.o
../access/nbtree/nbtsort.o
../access/nbtree/nbtutils.o
../access/nbtree/nbtxlog.o
../access/transam/clog.o
../access/transam/distributedlog.o
../access/transam/filerepdefs.o
../access/transam/gp_changetracking_log.o
../access/transam/gp_distributed_log.o
../access/transam/gp_transaction_log.o
../access/transam/multixact.o
../access/transam/persistentendxactrec.o
../access/transam/persistentfilesysobjname.o
../access/transam/rmgr.o
../access/transam/slru.o
../access/transam/subtrans.o
../access/transam/transam.o
../access/transam/twophase.o
../access/transam/twophase_rmgr.o
../access/transam/varsup.o
../access/transam/xact.o
../access/transam/xlog.o
../access/transam/xlog_mm.o
../access/transam/xlogfuncs.o
../access/transam/xlogloc.o
../access/transam/xlogutils.o
../bootstrap/bootparse.o
../bootstrap/bootstrap.o
../catalog/aclchk.o
../catalog/aoblkdir.o
../catalog/aocatalog.o
../catalog/aoseg.o
../catalog/aovisimap.o
../catalog/caql/caqlanalyze.o
../catalog/caql/catquery.o
../catalog/caql/gram.o
../catalog/catalog.o
../catalog/core/catcore.o
../catalog/core/catcoretable.o
../catalog/dependency.o
../catalog/gp_fastsequence.o
../catalog/gp_global_sequence.o
../catalog/gp_persistent.o
../catalog/heap.o
../catalog/index.o
../catalog/indexing.o
../catalog/namespace.o
../catalog/pg_aggregate.o
../catalog/pg_appendonly.o
../catalog/pg_attribute_encoding.o
../catalog/pg_compression.o
../catalog/pg_constraint.o
../catalog/pg_conversion.o
../catalog/pg_depend.o
../catalog/pg_extprotocol.o
../catalog/pg_exttable.o
../catalog/pg_largeobject.o
../catalog/pg_namespace.o
../catalog/pg_operator.o
../catalog/pg_proc.o
../catalog/pg_proc_callback.o
../catalog/pg_shdepend.o
../catalog/pg_type.o
../catalog/quicklz_compression.o
../catalog/toasting.o
../cdb/cdbappendonlystorage.o
../cdb/cdbappendonlystorageformat.o
../cdb/cdbappendonlystorageread.o
../cdb/cdbappendonlystoragewrite.o
../cdb/cdbbackup.o
../cdb/cdbbufferedappend.o
../cdb/cdbbufferedread.o
../cdb/cdbcat.o
../cdb/cdbcellbuf.o
../cdb/cdbconn.o
../cdb/cdbcopy.o
../cdb/cdbdatabaseinfo.o
../cdb/cdbdirectopen.o
../cdb/cdbdisp.o
../cdb/cdbdispatchresult.o
../cdb/cdbdistributedsnapshot.o
../cdb/cdbdistributedxacts.o
../cdb/cdbdistributedxid.o
../cdb/cdbdoublylinked.o
../cdb/cdbdtxcontextinfo.o
../cdb/cdbexplain.o
../cdb/cdbfilerep.o
../cdb/cdbfilerepconnclient.o
../cdb/cdbfilerepconnserver.o
../cdb/cdbfilerepmirror.o
../cdb/cdbfilerepmirrorack.o
../cdb/cdbfilerepprimary.o
../cdb/cdbfilerepprimaryack.o
../cdb/cdbfilerepprimaryrecovery.o
../cdb/cdbfilerepresetpeerprocess.o
../cdb/cdbfilerepresyncmanager.o
../cdb/cdbfilerepresyncworker.o
../cdb/cdbfilerepservice.o
../cdb/cdbfts.o
../cdb/cdbgang.o
../cdb/cdbglobalsequence.o
../cdb/cdbgroup.o
../cdb/cdbhash.o
../cdb/cdbheap.o
../cdb/cdbllize.o
../cdb/cdblocaldistribxact.o
../cdb/cdbmaxdistributedxid.o
../cdb/cdbmirroredappendonly.o
../cdb/cdbmirroredbufferpool.o
../cdb/cdbmirroredfilesysobj.o
../cdb/cdbmirroredflatfile.o
../cdb/cdbmutate.o
../cdb/cdboidsync.o
../cdb/cdbpartindex.o
../cdb/cdbpartition.o
../cdb/cdbpath.o
../cdb/cdbpathlocus.o
../cdb/cdbpathtoplan.o
../cdb/cdbpersistentbuild.o
../cdb/cdbpersistentcheck.o
../cdb/cdbpersistentdatabase.o
../cdb/cdbpersistentfilespace.o
../cdb/cdbpersistentfilesysobj.o
../cdb/cdbpersistentrecovery.o
../cdb/cdbpersistentrelation.o
../cdb/cdbpersistentstore.o
../cdb/cdbpersistenttablespace.o
../cdb/cdbpgdatabase.o
../cdb/cdbplan.o
../cdb/cdbpullup.o
../cdb/cdbrelsize.o
../cdb/cdbresynchronizechangetracking.o
../cdb/cdbsetop.o
../cdb/cdbshareddoublylinked.o
../cdb/cdbsharedoidsearch.o
../cdb/cdbsreh.o
../cdb/cdbsrlz.o
../cdb/cdbsubplan.o
../cdb/cdbsubselect.o
../cdb/cdbtargeteddispatch.o
../cdb/cdbthreadlog.o
../cdb/cdbtimer.o
../cdb/cdbtm.o
../cdb/cdbtmutils.o
../cdb/cdbutil.o
../cdb/cdbvarblock.o
../cdb/cdbvars.o
../cdb/motion/cdbmotion.o
../cdb/motion/htupfifo.o
../cdb/motion/ic_common.o
../cdb/motion/ic_tcp.o
../cdb/motion/ic_udp.o
../cdb/motion/ic_udpifc.o
../cdb/motion/tupchunklist.o
../cdb/motion/tupser.o
../cdb/partitionselection.o
../commands/aggregatecmds.o
../commands/alter.o
../commands/analyze.o
../commands/analyzefuncs.o
../commands/async.o
../commands/cluster.o
../commands/comment.o
../commands/conversioncmds.o
../commands/copy.o
../commands/dbcommands.o
../commands/define.o
../commands/explain.o
../commands/extprotocolcmds.o
../commands/filespace.o
../commands/functioncmds.o
../commands/indexcmds.o
../commands/lockcmds.o
../commands/opclasscmds.o
../commands/operatorcmds.o
../commands/portalcmds.o
../commands/prepare.o
../commands/proclang.o
../commands/queue.o
../commands/schemacmds.o
../commands/sequence.o
../commands/tablecmds.o
../commands/tablespace.o
../commands/trigger.o
../commands/typecmds.o
../commands/user.o
../commands/vacuum.o
../commands/vacuumlazy.o
../commands/variable.o
../commands/view.o
../executor/execAmi.o
../executor/execAOCSScan.o
../executor/execAOScan.o
../executor/execBitmapAOScan.o
../executor/execBitmapHeapScan.o
../executor/execBitmapTableScan.o
../executor/execDML.o
../executor/execDynamicIndexScan.o
../executor/execDynamicScan.o
../executor/execGpmon.o
../executor/execGrouping.o
../executor/execHeapScan.o
../executor/execHHashagg.o
../executor/execIndexscan.o
../executor/execJunk.o
../executor/execMain.o
../executor/execProcnode.o
../executor/execQual.o
../executor/execScan.o
../executor/execTuples.o
../executor/execUtils.o
../executor/execWorkfile.o
../executor/functions.o
../executor/instrument.o
../executor/nodeAgg.o
../executor/nodeAppend.o
../executor/nodeAssertOp.o
../executor/nodeBitmapAnd.o
../executor/nodeBitmapAppendOnlyscan.o
../executor/nodeBitmapHeapscan.o
../executor/nodeBitmapIndexscan.o
../executor/nodeBitmapOr.o
../executor/nodeBitmapTableScan.o
../executor/nodeDML.o
../executor/nodeDynamicIndexscan.o
../executor/nodeDynamicTableScan.o
../executor/nodeExternalscan.o
../executor/nodeFunctionscan.o
../executor/nodeHash.o
../executor/nodeHashjoin.o
../executor/nodeIndexscan.o
../executor/nodeLimit.o
../executor/nodeMaterial.o
../executor/nodeMergejoin.o
../executor/nodeMotion.o
../executor/nodeNestloop.o
../executor/nodePartitionSelector.o
../executor/nodeRepeat.o
../executor/nodeResult.o
../executor/nodeRowTrigger.o
../executor/nodeSequence.o
../executor/nodeSetOp.o
../executor/nodeShareInputScan.o
../executor/nodeSort.o
../executor/nodeSplitUpdate.o
../executor/nodeSubplan.o
../executor/nodeSubqueryscan.o
../executor/nodeTableFunction.o
../executor/nodeTableScan.o
../executor/nodeTidscan.o
../executor/nodeUnique.o
../executor/nodeValuesscan.o
../executor/nodeWindow.o
../executor/spi.o
../executor/tstoreReceiver.o
../fts/fts.o
../fts/ftsfilerep.o
../fts/ftsprobe.o
../fts/ftssan.o
../gp_libpq_fe/fe-auth.o
../gp_libpq_fe/fe-connect.o
../gp_libpq_fe/fe-exec.o
../gp_libpq_fe/fe-misc.o
../gp_libpq_fe/fe-protocol3.o
../gp_libpq_fe/pqexpbuffer.o
../lib/dllist.o
../lib/stringinfo.o
../libpq/auth.o
../libpq/be-fsstubs.o
../libpq/be-secure.o
../libpq/crypt.o
../libpq/hba.o
../libpq/ip.o
../libpq/md5.o
../libpq/pg_sha2.o
../libpq/pqcomm.o
../libpq/pqformat.o
../libpq/pqsignal.o
../libpq/sha2.o
#../main/main.o
../nodes/bitmapset.o
../nodes/copyfuncs.o
../nodes/equalfuncs.o
../nodes/list.o
../nodes/makefuncs.o
../nodes/nodeFuncs.o
../nodes/nodes.o
../nodes/outfast.o
../nodes/outfuncs.o
../nodes/params.o
../nodes/print.o
../nodes/read.o
../nodes/readfast.o
../nodes/readfuncs.o
../nodes/tidbitmap.o
../nodes/value.o
../optimizer/path/allpaths.o
../optimizer/path/clausesel.o
../optimizer/path/costsize.o
../optimizer/path/equivclass.o
../optimizer/path/indxpath.o
../optimizer/path/joinpath.o
../optimizer/path/joinrels.o
../optimizer/path/orindxpath.o
../optimizer/path/pathkeys.o
../optimizer/path/tidpath.o
../optimizer/plan/createplan.o
../optimizer/plan/initsplan.o
../optimizer/plan/planagg.o
../optimizer/plan/plangroupext.o
../optimizer/plan/planmain.o
../optimizer/plan/planner.o
../optimizer/plan/planpartition.o
../optimizer/plan/planshare.o
../optimizer/plan/planwindow.o
../optimizer/plan/setrefs.o
../optimizer/plan/subselect.o
../optimizer/plan/transform.o
../optimizer/prep/prepjointree.o
../optimizer/prep/prepqual.o
../optimizer/prep/preptlist.o
../optimizer/prep/prepunion.o
../optimizer/util/clauses.o
../optimizer/util/joininfo.o
../optimizer/util/pathnode.o
../optimizer/util/plancat.o
../optimizer/util/predtest.o
../optimizer/util/relnode.o
../optimizer/util/restrictinfo.o
../optimizer/util/tlist.o
../optimizer/util/var.o
../optimizer/util/walkers.o
../parser/analyze.o
../parser/gram.o
../parser/keywords.o
../parser/kwlookup.o
../parser/parse_agg.o
../parser/parse_clause.o
../parser/parse_coerce.o
../parser/parse_cte.o
../parser/parse_expr.o
../parser/parse_func.o
../parser/parse_node.o
../parser/parse_oper.o
../parser/parse_partition.o
../parser/parse_relation.o
../parser/parse_target.o
../parser/parse_type.o
../parser/parse_utilcmd.o
../parser/parser.o
../parser/scansup.o
../port/atomics.o
../port/darwin/system.o
../port/dynloader.o
../port/pg_latch.o
../port/pg_sema.o
../port/pg_shmem.o
../postmaster/alertseverity.o
../postmaster/autostats.o
../postmaster/autovacuum.o
../postmaster/backoff.o
../postmaster/bgwriter.o
../postmaster/checkpoint.o
../postmaster/fork_process.o
../postmaster/perfmon.o
../postmaster/perfmon_segmentinfo.o
../postmaster/pgarch.o
../postmaster/pgstat.o
../postmaster/postmaster.o
../postmaster/primary_mirror_mode.o
../postmaster/primary_mirror_transition_client.o
../postmaster/sendalert.o
../postmaster/seqserver.o
../postmaster/syslogger.o
../regex/regcomp.o
../regex/regerror.o
../regex/regexec.o
../regex/regfree.o
../replication/basebackup.o
../replication/gp_libpqwalreceiver.o
../replication/repl_gram.o
../replication/syncrep.o
../replication/walreceiver.o
../replication/walreceiverfuncs.o
../replication/walsender.o
../rewrite/rewriteDefine.o
../rewrite/rewriteHandler.o
../rewrite/rewriteManip.o
../rewrite/rewriteRemove.o
../rewrite/rewriteSupport.o
../storage/buffer/buf_init.o
../storage/buffer/buf_table.o
../storage/buffer/bufmgr.o
../storage/buffer/freelist.o
../storage/buffer/localbuf.o
../storage/file/bfz.o
../storage/file/buffile.o
../storage/file/compress_nothing.o
../storage/file/compress_zlib.o
../storage/file/fd.o
../storage/file/gp_compress.o
../storage/freespace/freespace.o
../storage/ipc/ipc.o
../storage/ipc/ipci.o
../storage/ipc/pmsignal.o
../storage/ipc/procarray.o
../storage/ipc/procsignal.o
../storage/ipc/shmem.o
../storage/ipc/shmqueue.o
../storage/ipc/sinval.o
../storage/ipc/sinvaladt.o
../storage/large_object/inv_api.o
../storage/lmgr/deadlock.o
../storage/lmgr/lmgr.o
../storage/lmgr/lock.o
../storage/lmgr/lwlock.o
../storage/lmgr/proc.o
../storage/lmgr/s_lock.o
../storage/lmgr/spin.o
../storage/page/bufpage.o
../storage/page/itemptr.o
../storage/smgr/md.o
../storage/smgr/smgr.o
../storage/smgr/smgrtype.o
../tcop/dest.o
../tcop/fastpath.o
../tcop/postgres.o
../tcop/pquery.o
../tcop/utility.o
../utils/adt/acl.o
../utils/adt/array_userfuncs.o
../utils/adt/arrayfuncs.o
../utils/adt/arrayutils.o
../utils/adt/ascii.o
../utils/adt/bool.o
../utils/adt/cash.o
../utils/adt/char.o
../utils/adt/date.o
../utils/adt/datetime.o
../utils/adt/datum.o
../utils/adt/dbsize.o
../utils/adt/domains.o
../utils/adt/encode.o
../utils/adt/float.o
../utils/adt/format_type.o
../utils/adt/formatting.o
../utils/adt/genfile.o
../utils/adt/geo_ops.o
../utils/adt/geo_selfuncs.o
../utils/adt/gp_optimizer_functions.o
../utils/adt/gp_partition_functions.o
../utils/adt/inet_cidr_ntop.o
../utils/adt/inet_net_pton.o
../utils/adt/int.o
../utils/adt/int8.o
../utils/adt/interpolate.o
../utils/adt/like.o
../utils/adt/lockfuncs.o
../utils/adt/mac.o
../utils/adt/matrix.o
../utils/adt/misc.o
../utils/adt/nabstime.o
../utils/adt/name.o
../utils/adt/network.o
../utils/adt/not_in.o
../utils/adt/numeric.o
../utils/adt/numutils.o
../utils/adt/oid.o
../utils/adt/oracle_compat.o
../utils/adt/percentile.o
../utils/adt/pg_locale.o
../utils/adt/pg_lzcompress.o
../utils/adt/pgstatfuncs.o
../utils/adt/pivot.o
../utils/adt/pseudotypes.o
../utils/adt/quote.o
../utils/adt/regexp.o
../utils/adt/regproc.o
../utils/adt/ri_triggers.o
../utils/adt/rowtypes.o
../utils/adt/ruleutils.o
../utils/adt/selfuncs.o
../utils/adt/tid.o
../utils/adt/timestamp.o
../utils/adt/uuid.o
../utils/adt/varbit.o
../utils/adt/varchar.o
../utils/adt/varlena.o
../utils/adt/version.o
../utils/adt/xid.o
../utils/adt/xml.o
../utils/cache/catcache.o
../utils/cache/inval.o
../utils/cache/lsyscache.o
../utils/cache/relcache.o
../utils/cache/sharedcache.o
../utils/cache/sharedcache_gclock.o
../utils/cache/syncrefhashtable.o
../utils/cache/syscache.o
../utils/cache/typcache.o
../utils/datumstream/datumstream.o
../utils/datumstream/datumstreamblock.o
../utils/error/assert.o
../utils/error/debugbreak.o
../utils/error/debugutils.o
../utils/error/elog.o
../utils/error/faultinject.o
../utils/error/pg_ltrace.o
../utils/fmgr/deprecated.o
../utils/fmgr/dfmgr.o
../utils/fmgr/fmgr.o
../utils/fmgr/funcapi.o
../utils/fmgrtab.o
../utils/gp/persistentutil.o
../utils/gp/segadmin.o
../utils/gpmon/gpmon.o
../utils/hash/dynahash.o
../utils/hash/hashfn.o
../utils/hash/pg_crc.o
../utils/init/flatfiles.o
../utils/init/globals.o
../utils/init/miscinit.o
../utils/init/postinit.o
../utils/mb/conv.o

../utils/mb/encnames.o
../utils/mb/mbutils.o
../utils/mb/wchar.o
../utils/mb/wstrcmp.o
../utils/mb/wstrncmp.o
../utils/misc/bitmap_compression.o
../utils/misc/bitstream.o
../utils/misc/faultinjector.o
../utils/misc/fstream/fstream.o
../utils/misc/fstream/gfile.o
../utils/misc/gp_atomic.o
../utils/misc/guc.o
../utils/misc/guc_gp.o
../utils/misc/help_config.o
../utils/misc/netcheck.o
../utils/misc/pg_rusage.o
../utils/misc/ps_status.o
../utils/misc/simex.o
../utils/misc/simexsys.o
../utils/misc/size.o
../utils/misc/superuser.o
../utils/misc/syncbitvector.o
../utils/misc/testutils.o
../utils/misc/tzparser.o
../utils/misc/uriparser.o
../utils/mmgr/aset.o
../utils/mmgr/asetDirect.o
../utils/mmgr/event_version.o
../utils/mmgr/idle_tracker.o
../utils/mmgr/mcxt.o
../utils/mmgr/memaccounting.o
../utils/mmgr/memprot.o
../utils/mmgr/mpool.o
../utils/mmgr/portalmem.o
../utils/mmgr/redzone_handler.o
../utils/mmgr/runaway_cleaner.o
../utils/mmgr/vmem_tracker.o
../utils/resowner/resowner.o
../utils/resscheduler/memquota.o
../utils/resscheduler/resqueue.o
../utils/resscheduler/resscheduler.o
../utils/session_state.o
../utils/sort/logtape.o
../utils/sort/tuplesort.o
../utils/sort/tuplesort_mk.o
../utils/sort/tuplesort_mkheap.o
../utils/sort/tuplesort_mkqsort.o
../utils/sort/tuplestore.o
../utils/sort/tuplestorenew.o
../utils/stat/bayes.o
../utils/stat/pinv.o
../utils/stat/regress.o
../utils/stat/student.o
../utils/time/combocid.o
../utils/time/tqual.o
../utils/workfile_manager/workfile_diskspace.o
../utils/workfile_manager/workfile_file.o
../utils/workfile_manager/workfile_mgr.o
../utils/workfile_manager/workfile_queryspace.o
../utils/workfile_manager/workfile_segmentspace.o
../../timezone/localtime.o
../../timezone/strftime.o
../../timezone/pgtz.o
../../timezone/gptime.o
../../../src/test/unit/mock/gpopt_mock.o

)
set_target_properties(
    hack PROPERTIES
    LINK_FLAGS "-L../../port -ldl -lbz2 -lxml2 -lz -lm -lcurl -lpgport -lpgport_srv"
    MACOSX_RPATH ON)
target_link_libraries(hack gpcodegen)

# Integrate with GPDB build system
# GPDB unit tests use unittest-check instead of test, so we add an alias target
# that calls the ctests we registered above.
add_custom_target(unittest-check ${CMAKE_CTEST_COMMAND})

SET(TEST_SOURCES
    tests/clang_compiler_unittest.cc
    tests/codegen_utils_unittest.cc
    tests/instance_method_wrappers_unittest.cc
    tests/codegen_manager_unittest.cc)

# Googletest framework for tests.
SET(GTEST_DIR ../../../gpAux/extensions/gtest)
add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)
include_directories(${GTEST_DIR}/include)
enable_testing()

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${TEST_SRC})
    target_link_libraries(${TEST_NAME} hack gpcodegen gtest)
    set_target_properties(${TEST_NAME}
        PROPERTIES
        LINK_FLAGS "-L../../port -ldl -lbz2 -lxml2 -lz -lm -lcurl -lpgport -lpgport_srv")
    add_test(${TEST_NAME} ${TEST_NAME})
    add_dependencies(unittest-check ${TEST_NAME})
endforeach()

# Examples
if (build_examples)
    add_subdirectory(example)
endif()

# Installation
install(TARGETS gpcodegen DESTINATION lib)
#install(DIRECTORY include/codegen DESTINATION include)

# Clean up
set(CMAKE_FILES Testing CMakeCache.txt CTestTestfile.cmake cmake_install.cmake install_manifest.txt Makefile)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_FILES}")
