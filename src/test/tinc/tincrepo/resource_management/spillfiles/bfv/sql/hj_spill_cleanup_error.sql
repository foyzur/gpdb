
SET statement_mem=5000;
SELECT COUNT(*) FROM (SELECT t1.* FROM test_spill_nodrop AS t1, test_spill_nodrop AS t2 WHERE t1.i1=t2.i2) T;
