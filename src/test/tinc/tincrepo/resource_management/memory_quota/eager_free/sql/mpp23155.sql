-- @author gcaragea
-- @created 2014-08-13 12:00:00
-- @modified 2014-08-13 12:00:00
-- @tags MPP-23155 bfv FEATURE_BRANCH_ONLY
-- @product_version gpdb: 4.3.2.0A, 4.3.2.0ORCA1, [4.3.4.0-], 4.3.99.99


-- start_ignore
drop table if exists mpp23155_t1; 
-- end_ignore
create table mpp23155_t1 (i1 int, i2 int) distributed by (i1);

set statement_mem='1500kB';
set gp_resqueue_memory_policy_auto_fixed_mem='1000kB';

set gp_resqueue_memory_policy=eager_free; 
select * from mpp23155_t1;

set gp_resqueue_memory_policy=auto; 
select * from mpp23155_t1;

-- start_ignore
drop table if exists mpp23155_t1; 
-- end_ignore
