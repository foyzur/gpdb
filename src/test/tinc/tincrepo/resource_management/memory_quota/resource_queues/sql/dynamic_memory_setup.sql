create database testdb_mpp16059;
create resource queue testrq_mpp16059 with (active_statements=3, memory_limit='3000MB');
create role testrole_mpp16059 with login;
alter role testrole_mpp16059 resource queue testrq_mpp16059;
