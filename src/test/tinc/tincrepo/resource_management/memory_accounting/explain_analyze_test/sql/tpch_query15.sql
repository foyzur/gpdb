-- @author balasr3
-- @description TPC-H query15
-- @created 2012-07-26 22:04:56
-- @modified 2012-07-26 22:04:56

explain analyze select
                s_suppkey,
                s_name,
                s_address,
                s_phone,
                total_revenue
        from
                supplier,
                revenue0
        where
                s_suppkey = supplier_no
                and total_revenue = (
                        select
                                max(total_revenue)
                        from
                                revenue0
                )
        order by
                s_suppkey;


