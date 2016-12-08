-- @author balasr3
-- @description TPC-H query03
-- @created 2012-07-26 22:04:56
-- @modified 2012-07-26 22:04:56
-- @tags tpch orca

BEGIN;
INSERT INTO region (r_name, r_comment) values ('QUERY EXECUTION', 'SAVEPOINT_NAME');
SAVEPOINT sp_SAVEPOINT_NAME;
INSERT INTO region (r_name, r_comment) values ('QUERY EXECUTION', 'inner_SAVEPOINT_NAME');

select
                l_orderkey,
                sum(l_extendedprice * (1 - l_discount)) as revenue,
                o_orderdate,
                o_shippriority
from
                customer,
                orders,
                lineitem
where
                c_mktsegment = 'BUILDING'
                and c_custkey = o_custkey
                and l_orderkey = o_orderkey
                and o_orderdate < date '1995-03-26'
                and l_shipdate > date '1995-03-26'
group by
                l_orderkey,
                o_orderdate,
                o_shippriority
order by
                revenue desc,
                o_orderdate
LIMIT 10;

RELEASE SAVEPOINT sp_SAVEPOINT_NAME;
COMMIT;
