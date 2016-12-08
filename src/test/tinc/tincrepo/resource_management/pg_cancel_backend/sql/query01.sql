-- @description cancel backend with query
-- @db_name gptest
-- @author tungs1
-- @modified 2013-11-17 12:00:00
-- @created 2013-11-17 12:00:00
-- @tags pg_cancel_backend HAWQ
-- @product version gpdb: [4.2.0.0, 9.9.9.9] && hawq: [1.1.1.0, 9.9.9.9]
-- @querylookup pg_cancel_backend_query01
select * from pg_cancel_backend_query01 a, pg_cancel_backend_query01 b, pg_cancel_backend_query01 c;
