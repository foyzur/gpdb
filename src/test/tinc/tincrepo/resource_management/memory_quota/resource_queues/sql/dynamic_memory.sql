\c testdb_mpp16059 testrole_mpp16059
DROP TABLE IF EXISTS T CASCADE;
CREATE TABLE T (i1 int, i2 int, i3 int, i4 int, i5 int, i6 int, i7 int, i8 int) distributed by (i1);
INSERT INTO T SELECT i, i, i, i, i, i, i, i % 10000 from generate_series(0,999999) i;

CREATE TABLE T2 (i1 int, i2 int, i3 int, i4 int, i5 int, i6 int, i7 int, i8 int) distributed by (i1);

DROP FUNCTION IF EXISTS DoInsert();
CREATE OR REPLACE FUNCTION DoInsert() RETURNS void AS
$BODY$
BEGIN
    EXECUTE 'INSERT INTO T2 SELECT T1.* FROM T AS T1, T AS T2 WHERE T1.i1 = T2.i2';
END
$BODY$
LANGUAGE 'plpgsql' ;

-- start_ignore
SET gp_workfile_type_hashjoin=buffile;
SET work_mem='2MB';
-- end_ignore
SET gp_workfile_limit_per_query='1MB';

SELECT DoInsert();

-- start_ignore
drop function DoInsert() cascade;
drop table t;
drop table t2;
-- end_ignore
