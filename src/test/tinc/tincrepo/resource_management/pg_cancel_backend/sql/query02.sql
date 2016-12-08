-- @description cancel backend with insert
-- @db_name gptest
-- @author tungs1
-- @modified 2013-11-17 12:00:00
-- @created 2013-11-17 12:00:00
-- @tags pg_cancel_backend
-- @product version gpdb: [4.2.0.0, 9.9.9.9]
-- @querylookup pg_cancel_backend_query02
update pg_cancel_backend_query02 set b = 10 where a > 0 AND b > 0 AND c > 0;
