drop table if exists mpp22698;

create table mpp22698(c1 int, c2 int) partition by range(c2) (start(1) end(6) every(1));