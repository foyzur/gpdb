-- @description cancel backend with copy
-- @db_name gptest
-- @author tungs1
-- @modified 2013-11-17 12:00:00
-- @created 2013-11-17 12:00:00
-- @tags pg_cancel_backend
-- @product version gpdb: [4.2.0.0, 9.9.9.9]
-- @querylookup pg_cancel_backend_query03
COPY pg_cancel_backend_query03 from '/tmp/query3.data' log errors into errortable segment reject limit 10;
