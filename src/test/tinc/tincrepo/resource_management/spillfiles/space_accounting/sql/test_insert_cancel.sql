
set gp_workfile_type_hashjoin=buffile; 
--SET gp_workfile_caching_loglevel="LOG"; 
set statement_mem=2048; 
set gp_autostats_mode = none;

drop table if exists t1_created; 
create table t1_created (i1 int, i2 int, i3 int, i4 int, i5 int, i6 int, i7 int, i8 int); 

begin; 

insert into t1_created
SELECT t1.* FROM hj_skew AS t1, hj_skew AS t2 WHERE t1.i1=t2.i2;

rollback; 

-- check used segspace after test
reset statement_mem; 
select * from mpp_23802_gp_workfile_segspace; 

--start_ignore
drop table if exists t1_created; 
--end_ignore
