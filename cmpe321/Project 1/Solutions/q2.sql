SELECT Drivers.year_of_birth, Sum(Drivers.total_points) as total_point FROM Drivers WHERE Drivers.year_of_birth BETWEEN '1990' AND '1999' GROUP BY Drivers.year_of_birth ORDER BY total_point DESC;

-- we can select the drivers that was born in 1990s by using between statement and we can sum the points of the drivers which were born in same year by grouping them according to their year_of_birth and with sum function
-- and finally we can sort them in descending order by using order by function