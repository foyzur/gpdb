-- @skip skipping indeterministic test
select pg_sleep(2);

select rqppriority, rqpquery from gp_toolkit.gp_resq_priority_statement where rqpquery ~~* 'FETCH FORWARD 50 FROM test;';

select current_query, application_name from pg_stat_activity where current_query ~~* 'FETCH FORWARD 50 FROM test;';
