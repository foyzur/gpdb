-- Adjust this setting to control where the objects get created.

CREATE SCHEMA workfile;
SET search_path = workfile;

BEGIN;

-- Register the functions.
     
CREATE OR REPLACE FUNCTION gp_workfile_mgr_test(testname text)
RETURNS setof bool
AS '$libdir/gp_workfile_mgr', 'gp_workfile_mgr_test_harness_wrapper' LANGUAGE C IMMUTABLE;

CREATE FUNCTION gp_workfile_mgr_test_allsegs(testname text)
RETURNS SETOF BOOL
AS
$$
 SELECT C.* FROM gp_toolkit.__gp_localid, workfile.gp_workfile_mgr_test($1) as C
 UNION ALL
 SELECT C.* FROM gp_toolkit.__gp_masterid, workfile.gp_workfile_mgr_test($1) as C;
$$
LANGUAGE SQL;

COMMIT;
