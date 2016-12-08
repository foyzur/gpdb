-- reset the segspace value
-- start_ignore
select * from mpp_23802_gp_workfile_mgr_reset_segspace;
-- end_ignore

----------
-- enable the fault injector now
\! gpfaultinjector -f exec_hashjoin_new_batch -y reset --seg_dbid 2
\! gpfaultinjector -f exec_hashjoin_new_batch -y interrupt --seg_dbid 2
