--start_ignore
revoke select on lineitem from max_0_pri_role;
revoke select on lineitem from low_0_pri_role;
revoke select on lineitem from low_1_pri_role;
revoke select on lineitem from low_2_pri_role;
revoke select on lineitem from low_3_pri_role;
drop user max_0_pri_role;
drop user low_0_pri_role;
drop user low_1_pri_role;
drop user low_2_pri_role;
drop user low_3_pri_role;
drop resource queue rq_max;
drop resource queue rq_low;
--end_ignore

-- Creating resource queue rq_max
create resource queue rq_max with (active_statements=100, priority=max);
-- Creating users with priority max
create user max_0_pri_role with nosuperuser resource queue rq_max;
alter user max_0_pri_role with resource queue rq_max;
grant select on lineitem to max_0_pri_role;
-- Creating resource queue rq_low
create resource queue rq_low with (active_statements=100, priority=low);
-- Creating users with priority low
create user low_0_pri_role with nosuperuser resource queue rq_low;
alter user low_0_pri_role with resource queue rq_low;
grant select on lineitem to low_0_pri_role;
-- Creating users with priority low
create user low_1_pri_role with nosuperuser resource queue rq_low;
alter user low_1_pri_role with resource queue rq_low;
grant select on lineitem to low_1_pri_role;
-- Creating users with priority low
create user low_2_pri_role with nosuperuser resource queue rq_low;
alter user low_2_pri_role with resource queue rq_low;
grant select on lineitem to low_2_pri_role;
-- Creating users with priority low
create user low_3_pri_role with nosuperuser resource queue rq_low;
alter user low_3_pri_role with resource queue rq_low;
grant select on lineitem to low_3_pri_role;

