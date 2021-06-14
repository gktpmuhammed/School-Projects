SELECT pum_name   FROM PUMs  
WHERE NOT EXISTS(SELECT * FROM Teams WHERE (PUMs.buyer_one=Teams.team_id OR PUMs.buyer_two=Teams.team_id OR PUMs.buyer_three=Teams.team_id OR PUMs.buyer_four=Teams.team_id) AND team_wc_count=0  )
GROUP BY pum_name;

-- We need a pum name whose all customers are world champion. So we should discard a PUM, if any customer is not world champion.
-- So we used WHERE NOT EXISTS function. We select PUMS if there does not exist such a customer that did not win wc. 
-- If wc count of one of the customers is 0 we did not select this pum in this way.