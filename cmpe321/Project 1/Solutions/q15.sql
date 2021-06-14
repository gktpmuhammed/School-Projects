SELECT short_name,
	sum(case when driver_id = driver_one then total_podiums end) DriverOne,
	sum(case when driver_id = driver_two then total_podiums end) DriverTwo
	FROM Teams JOIN Drivers on Drivers.driver_id=Teams.driver_one or Drivers.driver_id=Teams.driver_two GROUP BY short_name HAVING DriverOne >= 5*DriverTwo AND DriverOne!=0 ORDER BY short_name ASC;
	
-- We needed total podiums of drivers seperately. If we joined drivers and teams tables, we would have these values in seperate rows. But we need to have these values in same row but seperate columns.
-- So we used aliasing. With the help of CASE, we have this values in same row. Then we only compare these values, if driver one has 5 times more podium than driver two, we selected this team.
-- There was a case which both drivers do not have any podium. But this case was not in answer txt, so we controlled if driver one have 0 podiums and discard this result.
