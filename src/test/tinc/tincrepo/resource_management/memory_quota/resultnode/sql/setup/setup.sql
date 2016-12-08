create language plpythonu;

create or replace function get_operator_mem(query text) returns text as
$$
rv = plpy.execute(query)
list_opermem = []
for i in range(len(rv)):
    cur_line = rv[i]['QUERY PLAN']
    if 'Result' in cur_line or 'Function Scan on gp_partition_expansion' in cur_line or 'Function Scan on gp_partition_inverse' in cur_line:
    	operatorMemStr = cur_line[cur_line.rindex('(')+1:cur_line.rindex(')')]
    	list_opermem.append(operatorMemStr.split('=')[1])
return list_opermem
$$
language plpythonu;