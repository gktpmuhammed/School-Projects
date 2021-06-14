BEGIN TRANSACTION;
-- This table represents Database_manager and it has two field. First one is username which is the primary key and it must be unique and not null.
-- Second field is password and it can not be null. Also there is a check statement which limits the number of database managers by 5.
CREATE TABLE IF NOT EXISTS "Database_manager" (
	"username"	CHAR(50) NOT NULL UNIQUE,
	"password"	CHAR(50) NOT NULL,
	PRIMARY KEY("username"),
	CONSTRAINT "ROWID" CHECK("ROWID" <= 5)
);
-- This table stores the drug information. First field stores drug id and and its domain is char with length 7. It must be unique and must have a value. It is the primary key in this table.
-- Second field stores the drug name and its domain is char with max length 50. It must be unique and must have a value. 
-- Third field stores the chemical notation of drug and its domain is char with max length 200. It must be unique and must have a value. 
-- Fourth field stores the drugbank_id list that are interacted with our drug. And its domain is char with max length 200. It must have a value. 
-- Fifth field stores the decription of the drug and its domain is char with max length 2000. 
CREATE TABLE IF NOT EXISTS "Drug" (
	"drugbank_id"	CHAR(7) NOT NULL UNIQUE,
	"name"	CHAR(50) NOT NULL UNIQUE,
	"smiles"	CHAR(200) NOT NULL UNIQUE,
	"drug_interaction"	CHAR(200) NOT NULL,
	"description"	CHAR(2000),
	PRIMARY KEY("drugbank_id")
);
-- This table stores user information. First field stores username which is the primary key and it must be not null. Its domain is char with max length 50.
-- Second field stores institution names and its domain is char with max length 100. It must have a value. It is primary key for this table. 
-- Third field stores password and its domain is char with max length 50. It must have a value. 
CREATE TABLE IF NOT EXISTS "User" (
	"username"	CHAR(50) NOT NULL,
	"institution"	CHAR(100) NOT NULL,
	"password"	CHAR(50) NOT NULL,
	PRIMARY KEY("username","institution")
);
-- This table stores side effect ids of drugs and their names. First field stores side effect id and its domain is char with length 8. It must have a value and it must be unique. It is primary key for this table.
-- Second field stores side effect name and its domain is char with max length 100. It must have a value and it must be unique.
CREATE TABLE IF NOT EXISTS "Side_effect" (
	"umls_cui"	CHAR(8) NOT NULL UNIQUE,
	"side_effect_name"	CHAR(100) NOT NULL UNIQUE,
	PRIMARY KEY("umls_cui")
);
-- This table stores protein ids and their aminoacid sequence. First field stores protein id and its domain is char with length 6. It must have a value and it must be unique. It is primary key for this table.
-- Second field stores sequence of the protein and its domain is char with max length 5000. It must have a value and it must be unique.
CREATE TABLE IF NOT EXISTS "Proteins" (
	"uniprot_id"	CHAR(6) NOT NULL UNIQUE,
	"sequence"	CHAR(5000) NOT NULL UNIQUE,
	PRIMARY KEY("uniprot_id")
);
-- This table is the relation between side effects and drugs. First field is a primary and foreign key from Side_effect table.
-- Second field is a primary and foreign key from Drug table.
CREATE TABLE IF NOT EXISTS "Has" (
	"umls_cui"	CHAR(8) NOT NULL,
	"drugbank_id"	CHAR(7) NOT NULL,
	PRIMARY KEY("umls_cui","drugbank_id"),
	FOREIGN KEY("drugbank_id") REFERENCES "Drug"("drugbank_id"),
	FOREIGN KEY("umls_cui") REFERENCES "Side_effect"("umls_cui")
);
-- This table stores Article information. First field stores a link for the article and its domain is char with max length 100. It must have a value and it must be unique. It is primary key for this table.
-- Second field stores the authors of this article and its domain is char with max length 500. It must have a value.
-- Third field is a foreign key from User table and it stores the first author username that writes this article.
-- Fourth field is a foreign key from User table and it stores the institution name of the first author of this article. 
CREATE TABLE IF NOT EXISTS "Article" (
	"doi"	CHAR(100) NOT NULL UNIQUE,
	"authors"	CHAR(500) NOT NULL,
	"username"	CHAR(50) NOT NULL,
	"institution"	CHAR(100) NOT NULL,
	PRIMARY KEY("doi"),
	FOREIGN KEY("username") REFERENCES "User"("username"),
	FOREIGN KEY("institution") REFERENCES "User"("institution")
);
-- This table is the ternary relation between Drug, Proteins and Article tables. First field reaction_id is the primary field for this table and its domain is char with length 8. It must have a value and it must be unique.
-- Second field stores the meaasure of interaction and its domain is char with max length 4. It must have a value.
-- Third field stores the strength of the binding interaction between drugs and targets and its domain is real value. It must have a value.
-- Fourth field is a foreign key from Drug table and it stores the drugbank_id that will react with a protein.
-- Fifth field is a foreign key from Proteins table and it stores the protein id that will react with a drug.
-- Sixth field is a foreign key from Article table and it stores the article link that was written about this reaction.
CREATE TABLE IF NOT EXISTS "Reaction" (
	"reaction_id"	CHAR(8) NOT NULL UNIQUE,
	"measure"	CHAR(4) NOT NULL,
	"affinity_nM"	REAL NOT NULL,
	"drugbank_id"	CHAR(7) NOT NULL,
	"uniprot_id"	CHAR(6) NOT NULL,
	"doi"	CHAR(100) NOT NULL,
	PRIMARY KEY("reaction_id"),
	FOREIGN KEY("uniprot_id") REFERENCES "Proteins"("uniprot_id"),
	FOREIGN KEY("drugbank_id") REFERENCES "Drug"("drugbank_id"),
	FOREIGN KEY("doi") REFERENCES "Article"("doi")
);
COMMIT;
