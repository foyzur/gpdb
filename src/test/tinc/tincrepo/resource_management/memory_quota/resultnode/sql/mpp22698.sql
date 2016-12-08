-- @author guz4
-- @created 2014-08-12 12:00:00 
-- @modified 2014-08-12 12:00:00
-- @tags MPP-22698 bfv FEATURE_BRANCH_ONLY
-- @gucs gp_resqueue_print_operator_memory_limits=on
-- @product_version gpdb: 4.3.2.0A, 4.3.2.0ORCA1, 4.3.99.99, [5.0-]

-- test for gp_partition_propagation and gp_partition_expansion
select get_operator_mem('explain select * from mpp22698;');

-- test for gp_partition_selection
select get_operator_mem('explain select * from mpp22698 where c2 = 5;');

-- test for gp_partition_inverse
select get_operator_mem('explain select * from mpp22698 where c2 > 5;');