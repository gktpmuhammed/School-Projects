BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Drivers" (
	"driver_id"	INTEGER,
	"first_name"	TEXT,
	"last_name"	TEXT,
	"year_of_birth"	INTEGER,
	"country_of_birth"	TEXT,
	"country_of_driver_license"	TEXT,
	"team"	INTEGER,
	"driver_number"	INTEGER,
	"driver_wc_count"	INTEGER,
	"total_points"	INTEGER,
	"total_podiums"	INTEGER,
	"driver_hrf"	INTEGER,
	"driver_hrf_count"	INTEGER,
	FOREIGN KEY("team") REFERENCES "Teams"("team_id"),
	PRIMARY KEY("driver_id")
);
CREATE TABLE IF NOT EXISTS "GrandPrixs" (
	"grand_prix_id"	INTEGER,
	"grand_prix_name"	TEXT,
	"grand_prix_country"	TEXT,
	"circuit"	INTEGER,
	"starting_date"	TEXT,
	"ending_date"	TEXT,
	FOREIGN KEY("circuit") REFERENCES "Tracks"("track_id"),
	PRIMARY KEY("grand_prix_id")
);
CREATE TABLE IF NOT EXISTS "PUMs" (
	"pum_id"	INTEGER,
	"pum_name"	TEXT,
	"buyer_one"	INTEGER,
	"buyer_two"	INTEGER,
	"buyer_three"	INTEGER,
	"buyer_four"	INTEGER,
	FOREIGN KEY("buyer_one") REFERENCES "Teams"("team_id"),
	FOREIGN KEY("buyer_three") REFERENCES "Teams"("team_id"),
	FOREIGN KEY("buyer_two") REFERENCES "Teams"("team_id"),
	FOREIGN KEY("buyer_four") REFERENCES "Teams"("team_id"),
	PRIMARY KEY("pum_id")
);
CREATE TABLE IF NOT EXISTS "Teams" (
	"team_id"	INTEGER,
	"short_name"	TEXT,
	"full_name"	TEXT,
	"base_location"	TEXT,
	"team_chief"	TEXT,
	"technical_chief"	TEXT,
	"chassis"	TEXT,
	"pum"	INTEGER,
	"team_wc_count"	INTEGER,
	"team_hrf"	INTEGER,
	"team_hrf_count"	INTEGER,
	"driver_one"	INTEGER,
	"driver_two"	INTEGER,
	FOREIGN KEY("driver_two") REFERENCES "Drivers"("driver_id"),
	FOREIGN KEY("driver_one") REFERENCES "Drivers"("driver_id"),
	FOREIGN KEY("pum") REFERENCES "PUMs"("pum_id"),
	PRIMARY KEY("team_id")
);
CREATE TABLE IF NOT EXISTS "Tracks" (
	"track_id"	INTEGER,
	"circuit_name"	TEXT,
	"circuit_country"	TEXT,
	"circuit_type"	TEXT,
	"clim"	INTEGER,
	"total_laps"	INTEGER,
	"lrims"	INTEGER,
	"lap_recorder"	INTEGER,
	FOREIGN KEY("lap_recorder") REFERENCES "Drivers"("driver_id"),
	PRIMARY KEY("track_id")
);
INSERT INTO "Drivers" ("driver_id","first_name","last_name","year_of_birth","country_of_birth","country_of_driver_license","team","driver_number","driver_wc_count","total_points","total_podiums","driver_hrf","driver_hrf_count") VALUES (1,'Lewis','Hamilton',1985,'England','England',1,44,7,3778,165,1,95),
 (2,'Valtteri','Bottas',1989,'Finland','Finland',1,77,0,1512,56,1,9),
 (3,'Max','Verstappen',1997,'Belgium','Netherlands',2,33,0,1162,42,1,10),
 (4,'Sergio','Perez',1990,'Mexico','Mexico',2,11,0,706,10,1,1),
 (5,'Lando','Norris',1999,'England','England',3,4,0,146,1,3,1),
 (6,'Daniel','Ricciardo',1989,'Australia','Australia',3,3,0,1159,31,1,7),
 (7,'Lance','Stroll',1998,'Canada','Canada',4,18,0,142,3,3,3),
 (8,'Sebastian','Vettel',1987,'Germany','Germany',4,5,4,3018,121,1,53),
 (9,'Esteban','Ocon',1996,'Normandy','France',5,31,0,198,1,2,1),
 (10,'Fernando','Alonso',1981,'Spain','Spain',5,14,2,1899,97,1,32),
 (11,'Charles','Leclerc',1997,'Monaco','Monaco',6,16,0,401,12,1,2),
 (12,'Carlos','Sainz',1994,'Spain','Spain',6,55,0,372,2,2,1),
 (13,'Pierre','Gasly',1996,'France','France',7,10,0,199,2,1,1),
 (14,'Yuki','Tsunoda',2000,'Japan','Japan',7,22,0,0,0,21,0),
 (15,'Kimi','Raikkonen',1979,'Finland','Finland',8,7,1,1863,103,1,21),
 (16,'Antonio','Giovinazzi',1993,'Italy','Italy',8,99,0,18,0,5,1),
 (17,'Mick','Schumacher',1999,'Switzerland','Germany',9,47,0,0,0,21,0),
 (18,'Nikita','Mazepin',1999,'Russia','Russia',9,9,0,0,0,21,0),
 (19,'George','Russell',1998,'England','England',10,63,0,3,0,9,1),
 (20,'Nicholas','Latifi',1995,'Canada','Canada',10,6,0,0,0,11,3);
INSERT INTO "GrandPrixs" ("grand_prix_id","grand_prix_name","grand_prix_country","circuit","starting_date","ending_date") VALUES (1,'Formula 1 Gulf Air Bahrain Grand Prix 2021','Bahrein',1,'2021-03-26 00:00:00.000','2021-03-28 23:59:00.000'),
 (2,'Formula 1 Pirelli Gran Premio Del Made In Italy E Dell''emilia Romagna 2021','Italy',2,'2021-04-16 00:00:00.000','2021-04-18 23:59:00.000'),
 (3,'Formula 1 Heineken Grande Prémio De Portugal 2021','Portugal',3,'2021-04-30 00:00:00.000','2021-05-02 23:59:00.000'),
 (4,'Formula 1 Aramco Gran Premio De Espana 2021','Spain',4,'2021-05-07 00:00:00.000','2021-05-09 23:59:00.000'),
 (5,'Formula 1 Grand Prix De Monaco 2021','Monaco',5,'2021-05-20 00:00:00.000','2021-05-23 23:59:00.000'),
 (6,'Formula 1 Azerbaijan Grand Prix 2021','Azerbaijan',6,'2021-06-04 00:00:00.000','2021-06-06 23:59:00.000'),
 (7,'Formula 1 Heineken Grand Prix Du Canada 2021','Canada',7,'2021-06-11 00:00:00.000','2021-06-13 23:59:00.000'),
 (8,'Formula 1 Emirates Grand Prix De France 2021','France',8,'2021-06-25 00:00:00.000','2021-06-27 23:59:00.000'),
 (9,'Formula 1 myWorld Grosser Preis Von Österreich 2021','Austria',9,'2021-07-02 00:00:00.000','2021-07-04 23:59:00.000'),
 (10,'Formula 1 Pirelli British Grand Prix 2021','England',10,'2021-07-16 00:00:00.000','2021-07-18 23:59:00.000'),
 (11,'Formula 1 Magyar Nagydij 2021','Hungary',11,'2021-07-30 00:00:00.000','2021-08-01 23:59:00.000'),
 (12,'Formula 1 Rolex Belgian Grand Prix 2021','Belgium',12,'2021-08-27 00:00:00.000','2021-08-29 23:59:00.000'),
 (13,'Formula 1 Heineken Dutch Grand Prix 2021','Netherlands',13,'2021-09-03 00:00:00.000','2021-09-05 23:59:00.000'),
 (14,'Formula 1 Heineken Gran Premio D’italia 2021','Italy',14,'2021-09-10 00:00:00.000','2021-09-12 23:59:00.000'),
 (15,'Formula 1 VTB Russian Grand Prix 2021','Russia',15,'2021-09-24 00:00:00.000','2021-09-26 23:59:00.000'),
 (16,'Formula 1 Singapore Airlines Singapore Grand Prix 2021','Singapore',16,'2021-10-01 00:00:00.000','2021-10-03 23:59:00.000'),
 (17,'Formula 1 Japanese Grand Prix 2021','Japan',17,'2021-10-08 00:00:00.000','2021-10-10 23:59:00.000'),
 (18,'Formula 1 Aramco United States Grand Prix 2021','United States',18,'2021-10-22 00:00:00.000','2021-10-24 23:59:00.000'),
 (19,'Formula 1 Gran Premio De La Ciudad De Mexico 2021','Mexico',19,'2021-10-29 00:00:00.000','2021-10-31 23:59:00.000'),
 (20,'Formula 1 Heineken Grande Premio De Sao Paulo 2021','Brazil',20,'2021-11-05 00:00:00.000','2021-11-07 23:59:00.000'),
 (21,'Formula 1 Rolex Australian Grand Prix 2021','Australia',21,'2021-11-19 00:00:00.000','2021-11-21 23:59:00.000'),
 (22,'Formula 1 Saudi Arabian Grand Prix 2021','Saudi Arabia',22,'2021-12-03 00:00:00.000','2021-12-05 23:59:00.000'),
 (23,'Formula 1 Etihad Airways Abu Dhabi Grand Prix 2021','Abu Dhabi',23,'2021-12-10 00:00:00.000','2021-12-12 23:59:00.000');
INSERT INTO "PUMs" ("pum_id","pum_name","buyer_one","buyer_two","buyer_three","buyer_four") VALUES (1,'Mercedes',1,3,4,10),
 (2,'Honda',2,7,NULL,NULL),
 (3,'Renault',5,NULL,NULL,NULL),
 (4,'Ferrari',6,8,9,NULL);
INSERT INTO "Teams" ("team_id","short_name","full_name","base_location","team_chief","technical_chief","chassis","pum","team_wc_count","team_hrf","team_hrf_count","driver_one","driver_two") VALUES (1,'Mercedes','Mercedes-AMG Petronas F1 Team','England','Toto Wolff','James Allison','W12',1,7,1,106,1,2),
 (2,'Red Bull Racing','Red Bull Racing Honda','England','Christian Horner','Pierre Wache','RB16B',2,4,1,64,3,4),
 (3,'McLaren','McLaren F1 Team','England','Andreas Seidi','James Key','MCL35M',1,8,1,182,6,5),
 (4,'Aston Martin','Aston Marting Cognizant F1 Team','England','Otmar Szaufner','Andrew Green','AMR21',1,0,1,1,8,7),
 (5,'Alpine','Alpine F1 Team','England','Davide Brivio','Pat Fry','A521',3,2,1,20,10,9),
 (6,'Ferrari','Scuderia Mission Winnow Ferrari','Italy','Mattia Binotto','Enrico Cardile','SF21',4,16,1,239,11,12),
 (7,'AlphaTauri','Scuderia AlphaTauri Honda','Italy','Franz Tost','Jody Egginton','AT02',2,0,1,2,13,14),
 (8,'Alfa Romeo Racing','Alfa Romeo Racing ORLEN','Switzerland','Frederic Vasseur','Jan Monchaux','C41',4,0,1,1,15,16),
 (9,'Haas F1 Team','Uralkali Haas F1 Team','United States','Guenther Steiner','Simone Resta','VF-21',4,0,4,1,17,18),
 (10,'Williams','Williams Racing','England','Simon Roberta','Franchois-Xavier Demaison','FW43B',1,9,1,114,19,20);
INSERT INTO "Tracks" ("track_id","circuit_name","circuit_country","circuit_type","clim","total_laps","lrims","lap_recorder") VALUES (1,'Bahrain International Circuit','Bahrein','race',5412,57,91447,NULL),
 (2,'Autodromo Enzo E Dino Ferrari','Italy','race',4909,63,75484,1),
 (3,'Autodromo Internacional Do Algarve','Portugal','race',4653,66,78750,1),
 (4,'Circuit De Barcelona-Catalunya','Spain','race',4675,66,NULL,NULL),
 (5,'Circuit De Monaco','Monaco','street',3337,78,74260,3),
 (6,'Baku City Circuit','Azerbaijan','street',6003,51,103009,11),
 (7,'Circuit Gilles-Villeneuve','Canada','street',4361,70,73078,2),
 (8,'Circuit Paul Ricard','France','race',5842,53,92740,8),
 (9,'Red Bull Ring','Austria','race',4318,71,65619,12),
 (10,'Silverstone Circuit','England','race',5891,52,87097,3),
 (11,'Hungaroring','Hungary','race',4381,70,76627,1),
 (12,'Circuit De Spa-Francorchamps','Belgium','race',7004,44,106286,2),
 (13,'Circuit Zandvoort','Netherlands','race',4259,72,NULL,NULL),
 (14,'Autodromo Nazionale Monza','Italy','race',5793,53,81046,NULL),
 (15,'Sochi Autodrom','Russia','race',5848,53,95761,1),
 (16,'Marina Bay Street Circuit','Singapore','street',5063,61,101905,NULL),
 (17,'Suzuka International Racing Course','Japan','race',5807,53,90983,1),
 (18,'Circuit of the Americas','United States','race',5513,56,96169,11),
 (19,'Autodromo Hermanos Rodriguez','Mexico','race',4304,71,78741,2),
 (20,'Autodromo Jose Carlos Pace','Brazil','race',4309,71,70540,2),
 (21,'Melbourne Grand Prix Circuit','Australia','street',5303,58,84125,NULL),
 (22,'Jeddah Street Circuit','Saudi Arabia','street',6175,50,NULL,NULL),
 (23,'Yas Marina Circuit','Abu Dhabi','race',5554,55,99283,1);
COMMIT;