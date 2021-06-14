SELECT short_name, base_location, Drivers.first_name, Drivers.country_of_birth, GROUP_CONCAT(Tracks.circuit_country, ',') as circuitCountry 
FROM Tracks JOIN Drivers on tracks.lap_recorder=Drivers.driver_id 
JOIN Teams on Drivers.team=Teams.team_id  
GROUP BY first_name HAVING(circuitCountry like 'France' or circuitCountry like 'Monaco') ORDER BY Teams.short_name asc;

-- The question said "List the short names and base locations of the teams that contain at least one driver who holds a lap record only in France or Monaco track."
-- We needed Tracks, Drivers and Teams tables for the requirements and we only need lap recorder drivers. So we joined this tables in order to have only lap recorder drivers.
-- We struggled in this question. Because we need to be sure pilot is only lap recorder in Monaco or France, nowhere else. 
-- So we used group_concat to concatenate every circuit_country which our pilot is lap recorder. We use group by first name for this.
-- Lastly we used having function to control only france or monaco records.