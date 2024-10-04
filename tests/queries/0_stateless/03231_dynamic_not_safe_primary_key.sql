SET allow_experimental_dynamic_type = 1;
DROP TABLE IF EXISTS t0;
DROP TABLE IF EXISTS t1;
CREATE TABLE t0 (c0 Int) ENGINE = AggregatingMergeTree() ORDER BY (c0);
CREATE TABLE t1 (c0 Array(Dynamic), c1 Int) ENGINE = MergeTree() ORDER BY (c0);
INSERT INTO t1 (c0, c1) VALUES ([18446717433683171873], 13623876564923702671), ([-4], 6111684076076982207);
SELECT 1 FROM t0 FINAL JOIN t1 ON TRUE;
DROP TABLE t0;
DROP TABLE t1;

