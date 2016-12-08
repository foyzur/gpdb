--start_ignore
REVOKE ALL PRIVILEGES ON PUBLIC.orders FROM testuser1;
DROP USER IF EXISTS testuser1;
DROP RESOURCE QUEUE resourcequeue1;
--end_ignore

create user testuser1 with nosuperuser nocreatedb nocreaterole;
GRANT ALL PRIVILEGES ON PUBLIC.orders TO testuser1;
create resource queue resourcequeue1 active threshold 10;
alter role testuser1 with resource queue resourcequeue1;

