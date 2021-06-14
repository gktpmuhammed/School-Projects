SELECT Teams.short_name, Teams.full_name, Teams.team_wc_count , sum(Drivers.total_podiums) AS total_podium FROM Teams INNER JOIN Drivers ON Drivers.team=Teams.team_id WHERE EXISTS(SELECT * FROM Drivers WHERE Drivers.team=Teams.team_id AND Drivers.country_of_birth!=Drivers.country_of_driver_license) GROUP BY Drivers.team HAVING total_podium>=10  ORDER BY short_name DESC ; 

-- First we found the drivers which country_of_birth and country_of_driver_license are different. FROM these drivers we can found the teams that we are looking for. With sum function we found the total_podiums of the teams. 
-- By using having function we eliminated the teams which have 10 podiums or less. lastly, we sort the result by Teams.short_name in descending order. 
