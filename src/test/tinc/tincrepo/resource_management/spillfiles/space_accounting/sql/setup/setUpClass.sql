-- @product_version gpdb: [4.3.3.0-], [4.2.8.1-4.2]
-- create view to read the segspace value

DROP VIEW IF EXISTS mpp_23802_gp_workfile_segspace;
DROP FUNCTION IF EXISTS mpp_23802_gp_workfile_segspace_f();

DROP VIEW IF EXISTS mpp_23802_gp_workfile_mgr_reset_segspace;
DROP FUNCTION IF EXISTS mpp_23802_gp_workfile_mgr_reset_segspace_f();


CREATE FUNCTION mpp_23802_gp_workfile_segspace_f()
RETURNS SETOF record
AS '$libdir/gp_workfile_mgr', 'gp_workfile_mgr_used_diskspace'
LANGUAGE C IMMUTABLE;


CREATE VIEW mpp_23802_gp_workfile_segspace AS
SELECT C.*
FROM gp_toolkit.__gp_localid, mpp_23802_gp_workfile_segspace_f() AS C (
segid int,
size bigint
)
UNION ALL
SELECT C.*
FROM gp_toolkit.__gp_masterid, mpp_23802_gp_workfile_segspace_f() AS C (
segid int,
size bigint
);


-- create helper UDF to reset the segpsace variable
CREATE FUNCTION mpp_23802_gp_workfile_mgr_reset_segspace_f()
RETURNS SETOF bigint
AS '$libdir/gp_workfile_mgr', 'gp_workfile_mgr_reset_segspace'
LANGUAGE C IMMUTABLE;

CREATE VIEW mpp_23802_gp_workfile_mgr_reset_segspace AS
SELECT * FROM gp_toolkit.__gp_localid, mpp_23802_gp_workfile_mgr_reset_segspace_f() 
UNION ALL
SELECT * FROM gp_toolkit.__gp_masterid, mpp_23802_gp_workfile_mgr_reset_segspace_f();


--- create and populate the table

DROP TABLE IF EXISTS hj_skew;
CREATE TABLE hj_skew (i1 int, i2 int, i3 int, i4 int, i5 int, i6 int, i7 int, i8 int); 

set gp_autostats_mode = none;

-- many values with i1 = 1
INSERT INTO hj_skew SELECT 1,i,i,i,i,i,i,i FROM generate_series (0,1379999) i; 
-- some nicely distributed values
INSERT INTO hj_skew SELECT i,i,i,i,i,i,i,i FROM generate_series (0,2679999) i; 

ANALYZE hj_skew;
