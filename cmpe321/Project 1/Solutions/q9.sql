SELECT ABS(sum(CASE 
                WHEN year_of_birth BETWEEN '1980' AND '1989' THEN -total_points 
                WHEN year_of_birth BETWEEN '1990' AND '1999' THEN total_points END)) as Difference FROM Drivers;

-- We need to find difference of total points between olds and youngs. So we used CASE in order to sum those total points according to year_of_births. 
-- We can find these values seperately in this way. We also used ABS to be sure result is positive.
