SELECT  Teams.full_name as short_name, Teams.team_chief, Teams.technical_chief, Teams.team_wc_count FROM Teams JOIN PUMs ON Teams.short_name=PUMs.pum_name ORDER BY team_wc_count DESC;

-- we need to join the teams table with PUMs.pum_name field which is renamed by short_name and select full_name team_chief technical_chief and team_wc_count from newly created table. 
-- Thus we can find the teams which are using the power unit manufactured by them. In other words we select the teams that has the same name with their power unit manufacturer. 

