CREATE TEMPORARY TABLE t0 (c0 Int TTL defaultProfiles()) ENGINE = MergeTree ORDER BY tuple(); -- { serverError BAD_ARGUMENTS }
