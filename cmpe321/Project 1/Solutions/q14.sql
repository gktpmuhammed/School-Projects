SELECT short_name, team_hrf_count, sum(Drivers.driver_hrf_count) as drivershrf 
FROM Teams JOIN Drivers 
WHERE(Drivers.driver_id = Teams.driver_one or Drivers.driver_id = Teams.driver_two ) 
GROUP BY short_name HAVING drivershrf > team_hrf_count ORDER BY short_name DESC;

-- We only needed to compare drivers' hrf count and team's hrf count. We summed up drivers' hrf count with sum function and group by short_name. We calculated total hrf count of drivers of that team.
-- We already have team's hrf count so only compare these values with having function. Then descending order by short_name.