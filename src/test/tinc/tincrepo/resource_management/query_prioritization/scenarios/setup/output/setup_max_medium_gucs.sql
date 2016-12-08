--start_ignore
revoke select on lineitem from max_0_pri_role;
revoke select on lineitem from medium_0_pri_role;
revoke select on lineitem from medium_1_pri_role;
revoke select on lineitem from medium_2_pri_role;
revoke select on lineitem from medium_3_pri_role;
drop user max_0_pri_role;
drop user medium_0_pri_role;
drop user medium_1_pri_role;
drop user medium_2_pri_role;
drop user medium_3_pri_role;
drop resource queue rq_max;
drop resource queue rq_medium;
--end_ignore

-- Creating resource queue rq_max
create resource queue rq_max with (active_statements=100, priority=max);
-- Creating users with priority max
create user max_0_pri_role with nosuperuser resource queue rq_max;
alter user max_0_pri_role with resource queue rq_max;
grant select on lineitem to max_0_pri_role;
-- Creating resource queue rq_medium
create resource queue rq_medium with (active_statements=100, priority=medium);
-- Creating users with priority medium
create user medium_0_pri_role with nosuperuser resource queue rq_medium;
alter user medium_0_pri_role with resource queue rq_medium;
grant select on lineitem to medium_0_pri_role;
-- Creating users with priority medium
create user medium_1_pri_role with nosuperuser resource queue rq_medium;
alter user medium_1_pri_role with resource queue rq_medium;
grant select on lineitem to medium_1_pri_role;
-- Creating users with priority medium
create user medium_2_pri_role with nosuperuser resource queue rq_medium;
alter user medium_2_pri_role with resource queue rq_medium;
grant select on lineitem to medium_2_pri_role;
-- Creating users with priority medium
create user medium_3_pri_role with nosuperuser resource queue rq_medium;
alter user medium_3_pri_role with resource queue rq_medium;
grant select on lineitem to medium_3_pri_role;

