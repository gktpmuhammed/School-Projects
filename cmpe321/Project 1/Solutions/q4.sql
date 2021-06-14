SELECT Sum(Drivers.total_points) FROM Drivers WHERE Drivers.country_of_birth = Drivers.country_of_driver_license AND Drivers.team IN (SELECT Teams.team_id FROM Teams WHERE Teams.base_location = 'England' AND Teams.pum <> '1');

-- First we found the teams which have the base_location in England and have Mercedes as their PUMs. And then we can select the drivers from these teams and also we need to check whether the country_of_birth and country_of_driver_license are the same or not.
-- then we can sum the points of these drivers which are meeting these conditions.