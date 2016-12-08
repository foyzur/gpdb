--start_ignore
revoke select on lineitem from max_0_pri_role;
revoke select on lineitem from min_0_pri_role;
revoke select on lineitem from min_1_pri_role;
revoke select on lineitem from min_2_pri_role;
revoke select on lineitem from min_3_pri_role;
drop user max_0_pri_role;
drop user min_0_pri_role;
drop user min_1_pri_role;
drop user min_2_pri_role;
drop user min_3_pri_role;
drop resource queue rq_max;
drop resource queue rq_min;
--end_ignore

-- Creating resource queue rq_max
create resource queue rq_max with (active_statements=100, priority=max);
-- Creating users with priority max
create user max_0_pri_role with nosuperuser resource queue rq_max;
alter user max_0_pri_role with resource queue rq_max;
grant select on lineitem to max_0_pri_role;
-- Creating resource queue rq_min
create resource queue rq_min with (active_statements=100, priority=min);
-- Creating users with priority min
create user min_0_pri_role with nosuperuser resource queue rq_min;
alter user min_0_pri_role with resource queue rq_min;
grant select on lineitem to min_0_pri_role;
-- Creating users with priority min
create user min_1_pri_role with nosuperuser resource queue rq_min;
alter user min_1_pri_role with resource queue rq_min;
grant select on lineitem to min_1_pri_role;
-- Creating users with priority min
create user min_2_pri_role with nosuperuser resource queue rq_min;
alter user min_2_pri_role with resource queue rq_min;
grant select on lineitem to min_2_pri_role;
-- Creating users with priority min
create user min_3_pri_role with nosuperuser resource queue rq_min;
alter user min_3_pri_role with resource queue rq_min;
grant select on lineitem to min_3_pri_role;

