SET gp_workfile_caching_loglevel="LOG";
\! gpfaultinjector -f workfile_cleanup_set -y reset --seg_dbid 2
\! gpfaultinjector -f workfile_cleanup_set -y interrupt --seg_dbid 2

