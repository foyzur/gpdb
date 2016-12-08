REVOKE ALL PRIVILEGES ON PUBLIC.orders FROM @user_name@;

DROP USER IF EXISTS @user_name@;

create user @user_name@ with nosuperuser nocreatedb nocreaterole;

GRANT ALL PRIVILEGES ON PUBLIC.orders TO @user_name@;

DROP RESOURCE QUEUE @resource_queue@;

create resource queue @resource_queue@ active threshold @concurrency@;

alter resource queue @resource_queue@ with (memory_limit = '@memory_limit@');

alter role @user_name@ with resource queue @resource_queue@;

