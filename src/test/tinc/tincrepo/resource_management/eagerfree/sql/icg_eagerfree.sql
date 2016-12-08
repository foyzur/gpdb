-- @author garcic12
-- @created 2013-11-20 12:00:00
-- @modified 2013-11-20 12:00:00
-- @tags ci
-- @description Eagerfree test for subquery unnesting.

select * from smallt_part where i < any (select count(*) from smallt2_part where smallt_part.i = smallt2_part.i group by d);
