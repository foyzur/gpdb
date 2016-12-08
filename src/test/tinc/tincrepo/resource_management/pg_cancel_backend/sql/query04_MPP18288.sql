-- @description regression test for MPP-18288
-- @db_name gptest
-- @author tungs1
-- @modified 2013-11-17 12:00:00
-- @created 2013-11-17 12:00:00
-- @tags pg_cancel_backend HAWQ
-- @product version gpdb: [4.2.0.0, 9.9.9.9] && hawq: [1.1.1.0, 9.9.9.9]
-- @querylookup pg_cancel_backend_query04_MPP18288
set statement_mem="5MB";
explain analyze select i1,i2 from pg_cancel_backend_query04_MPP18288 order by i2;
