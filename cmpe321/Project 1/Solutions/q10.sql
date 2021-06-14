SELECT Drivers.driver_id, Drivers.first_name, Drivers.last_name, Tracks.circuit_name, Tracks.lrims 
From Tracks JOIN Drivers ON Drivers.driver_id = Tracks.lap_recorder 
INNER JOIN Teams on Drivers.team=Teams.team_id 
WHERE short_name = 'Red Bull Racing' ORDER BY lrims asc;

-- Firstly we selected Drivers.driver_id, Drivers.first_name, Drivers.last_name, Tracks.circuit_name, Tracks.lrims as said in question.
-- In order to find lap recorder Red Bull Racing pilots, we joined drivers and tracks tables. We can reach lap records from tracks and join only recorders with ON. 
-- We also joined Teams table because we need to find short name of the team. We checked this with where function and order by lrims with ascending order.