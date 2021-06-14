WITH table1 AS
(SELECT circuit_type, short_name, lap_recorder, circuit_type,
    sum(case when circuit_type like 'street' then 1 end) records_in_street,
    sum(case when circuit_type like 'race' then 1 end) records_in_race
    FROM Teams JOIN Tracks on Teams.driver_one=Tracks.lap_recorder or Teams.driver_two=Tracks.lap_recorder GROUP by short_name, circuit_type)

SELECT circuit_type, short_name, (coalesce(records_in_street,0)+coalesce(records_in_race,0)) as records  FROM table1 WHERE records_in_race IN 
( SELECT MAX(records_in_race)
  FROM table1
)
UNION
SELECT circuit_type, short_name, (coalesce(records_in_street,0)+coalesce(records_in_race,0)) as records  FROM table1 WHERE records_in_street IN 
( SELECT MAX(records_in_street)
  FROM table1
) ORDER BY circuit_type, short_name asc;

-- This question was hard to solve in one query. Because we need to calculate records in street and records in race seperately but we should write this values in one column.
-- So, we used a dummy table to calculate these values. We summed up record numbers seperately in this way. 
-- Then we select maximum valued rows from this table. We also combine record numbers in one column. We used coalesce because one of the columns are NULL and we need to take this value as 0.