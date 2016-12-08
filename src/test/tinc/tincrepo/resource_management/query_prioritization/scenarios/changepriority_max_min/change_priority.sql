select pg_sleep(5);
select rqpusename,rqppriority from gp_toolkit.gp_resq_priority_statement where rqpusename like '%_role' order by rqpusename;

select gp_adjust_priority((select rqpsession from gp_toolkit.gp_resq_priority_statement where rqpusename = 'min_0_pri_role'), (select rqpcommand from gp_toolkit.gp_resq_priority_statement where rqpusename = 'min_0_pri_role'), 'MAX');

select rqpusename,rqppriority from gp_toolkit.gp_resq_priority_statement where rqpusename like '%_role' order by rqpusename;
