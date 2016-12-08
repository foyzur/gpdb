--@concurrency 1
select pg_sleep(10);
select rsqname,rsqcountlimit,rsqcountvalue,rsqmemorylimit,rsqmemoryvalue,rsqwaiters,rsqholders from gp_toolkit.gp_resqueue_status where rsqname='resourcequeue1';
