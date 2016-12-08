set gp_resqueue_print_operator_memory_limits=on;

--setting additional confs

--with eager_free 
set gp_resqueue_memory_policy=eager_free;

--statement_mem=175MB
set statement_mem='175MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=275MB
set statement_mem='275MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=475MB
set statement_mem='475MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=75MB
set statement_mem='75MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=375MB
set statement_mem='375MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=525MB
set statement_mem='525MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=225MB
set statement_mem='225MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=325MB
set statement_mem='325MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=125MB
set statement_mem='125MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=425MB
set statement_mem='425MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--with auto 
set gp_resqueue_memory_policy=auto;

--statement_mem=175MB
set statement_mem='175MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=275MB
set statement_mem='275MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=475MB
set statement_mem='475MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=75MB
set statement_mem='75MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=375MB
set statement_mem='375MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=525MB
set statement_mem='525MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=225MB
set statement_mem='225MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=325MB
set statement_mem='325MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=125MB
set statement_mem='125MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


--statement_mem=425MB
set statement_mem='425MB';


explain analyze
--query with deep group tree and each group having just one memory intensive operator
select sum(FOO2.sum_qty) as sum_qty,
sum(FOO2.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO2.l_returnflag,
FOO2.l_linestatus,
FOO2.l_partkey
FROM
(
select sum(FOO1.sum_qty) as sum_qty,
sum(FOO1.sum_base_price) as sum_base_price,
count(*) as count_order,
FOO1.l_returnflag,
FOO1.l_linestatus,
FOO1.l_partkey
FROM
 ( select l_returnflag,l_linestatus,l_partkey,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
   from
   lineitem
   where
   l_shipdate <= date '1998-12-01' - interval '106 day'
   group by
   l_returnflag, l_linestatus,l_partkey
   order by l_partkey
 ) FOO1
group by FOO1.l_partkey,FOO1.l_returnflag,FOO1.l_linestatus
order by FOO1.l_partkey
) FOO2
group by FOO2.l_partkey,FOO2.l_returnflag,FOO2.l_linestatus
order by FOO2.l_partkey;


