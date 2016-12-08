set gp_resqueue_print_operator_memory_limits=on;

--setting additional confs

set enable_hashjoin=on;

set  enable_seqscan=off;

set  enable_indexscan=on;

set  enable_bitmapscan=on;

--with eager_free 
set gp_resqueue_memory_policy=eager_free;

--statement_mem=175MB
set statement_mem='175MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=275MB
set statement_mem='275MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=475MB
set statement_mem='475MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=75MB
set statement_mem='75MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=375MB
set statement_mem='375MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=525MB
set statement_mem='525MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=225MB
set statement_mem='225MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=325MB
set statement_mem='325MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=125MB
set statement_mem='125MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=425MB
set statement_mem='425MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--with auto 
set gp_resqueue_memory_policy=auto;

--statement_mem=175MB
set statement_mem='175MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=275MB
set statement_mem='275MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=475MB
set statement_mem='475MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=75MB
set statement_mem='75MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=375MB
set statement_mem='375MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=525MB
set statement_mem='525MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=225MB
set statement_mem='225MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=325MB
set statement_mem='325MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=125MB
set statement_mem='125MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


--statement_mem=425MB
set statement_mem='425MB';


explain analyze
--query with blocking operators that would require rescan.bitmapindex scan on l_shipdate will require rescan and hence it should not be considered blocking
with lineitem_stats as
(
select l_returnflag,l_linestatus,l_partkey,l_shipdate,
          sum(l_quantity) as sum_qty,sum(l_extendedprice) as sum_base_price,count(*) as count_order
from
   lineitem
where
    l_shipdate <= date '1998-12-01' - interval '106 day'
group by
    l_returnflag, l_linestatus,l_partkey,l_shipdate
order by l_partkey)

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where
    l_partkey = p_partkey and l_shipdate <=  date '1998-01-01'

UNION

select l_partkey,sum_qty,sum_base_price,count_order
from
   lineitem_stats,part
where 
    l_partkey = p_partkey and l_shipdate <= date '1997-01-01';


