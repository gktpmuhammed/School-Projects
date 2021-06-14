alter table user change user_name username text null;

alter table user change institutions institution text null;

alter table user modify username varchar(256) not null;

alter table user modify institution varchar(256) not null;

alter table user change passwords password text null;

alter table user
	add constraint user_pk
		primary key (username, institution);


rename table database_manager to databasemanager;

rename table uniprot to proteins;

create table drugs as select distinct d.drugbank_id, d.name, d.description, b.smiles 
from drugbank d left join bindingdb b on d.drugbank_id = b.drugbank_id;

alter table drugs modify drugbank_id char(7) not null;

create unique index drugs_drugbank_id_uindex
	on drugs (drugbank_id);

alter table drugs
	add constraint drugs_pk
		primary key (drugbank_id);

create table side_effect as select distinct umls_cui, side_effect_name from sider;

alter table side_effect modify umls_cui char(8) not null;

alter table side_effect modify side_effect_name text not null;

create unique index side_effect_umls_cui_uindex
	on side_effect (umls_cui);

alter table side_effect
	add constraint side_effect_pk
		primary key (umls_cui);

create table has as select umls_cui, drugbank_id from sider;

create table reaction as select reaction_id,drugbank_id,uniprot_id,target_name,measure,affinity_nM,doi from bindingdb;

create table article as select distinct doi,authors,institution from bindingdb;

alter table databasemanager modify username varchar(256) not null;

alter table databasemanager modify password text not null;

create unique index databasemanager_username_uindex
	on databasemanager (username);

alter table databasemanager
	add constraint databasemanager_pk
		primary key (username);

alter table has modify umls_cui char(8) not null;

alter table has modify drugbank_id char(7) not null;

alter table has
	add constraint has_drugs_drugbank_id_fk
		foreign key (drugbank_id) references drugs (drugbank_id);

alter table has
	add constraint has_side_effect_umls_cui_fk
		foreign key (umls_cui) references side_effect (umls_cui);

alter table has
	add constraint has_pk
		primary key (umls_cui, drugbank_id);


alter table proteins modify uniprot_id char(6) not null;

create unique index proteins_uniprot_id_uindex
	on proteins (uniprot_id);

alter table proteins
	add constraint proteins_pk
		primary key (uniprot_id);

alter table article modify doi varchar(256) not null;

alter table article modify institution varchar(256) not null;

create unique index article_doi_uindex
	on article (doi);

alter table article
	add constraint article_pk
		primary key (doi);

alter table reaction modify reaction_id char(8) not null;

alter table reaction modify drugbank_id char(7) not null;

alter table reaction modify uniprot_id char(6) not null;

alter table reaction modify doi varchar(256) not null;

create unique index reaction_reaction_id_uindex
	on reaction (reaction_id);

alter table reaction
	add constraint reaction_pk
		primary key (reaction_id);

alter table reaction
	add constraint reaction_drugs_drugbank_id_fk
		foreign key (drugbank_id) references drugs (drugbank_id);

alter table reaction
	add constraint reaction_proteins_uniprot_id_fk
		foreign key (uniprot_id) references proteins (uniprot_id)
			on delete cascade;

create table contributors as select doi from article group by doi;

alter table contributors add username varchar(256);

create index contributors_doi_index
	on contributors (doi);

create index contributors_username_index
	on contributors (username);

alter table contributors
	add constraint contributors_article_doi_fk
		foreign key (doi) references article (doi);

alter table contributors
	add constraint contributors_user_username_fk
		foreign key (username) references user (username);

create table institutions
    select distinct institution, (count(doi)*5 + (length(authors) - length(replace(authors, ';',''))+1)*2) as score
    from article group by institution order by score desc;

alter table institutions
	add constraint institutions_pk
		primary key (institution);

alter table article
	add constraint article_institutions_institution_fk
		foreign key (institution) references institutions (institution);

alter table user
	add constraint user_institutions_institution_fk
		foreign key (institution) references institutions (institution);

create table interactions
(
	drug1 char(7) null,
	drug2 char(7) null,
	constraint interactions_drugs_drugbank_id_fk
		foreign key (drug1) references drugs (drugbank_id),
	constraint interactions_drugs_drugbank_id_fk_2
		foreign key (drug2) references drugs (drugbank_id)
);


DELIMITER $$
create trigger trigger_articledelete_score after delete on article for each row
    begin
        update institutions set score = score -5 where institutions.institution=old.institution;
    end $$
DELIMITER ;

DELIMITER $$
create trigger trigger_articleadd_score after insert on article for each row
    begin
        update institutions set score = score +5 where institutions.institution=NEW.institution;
    end $$
DELIMITER ;

DELIMITER $$
create trigger trigger_contributoradd_score after insert on contributors for each row
    begin
        update institutions set score = score +2 where institution in (select article.institution from article where article.doi=NEW.doi);
    end $$
DELIMITER ;

DELIMITER $$
create trigger trigger_contributordelete_score before delete on contributors for each row
    begin
        update institutions set score = score -2 where institution in (select article.institution from article where article.doi=OLD.doi);
    end $$
DELIMITER ;

DELIMITER $$
create trigger trigger_protein_delete before delete on proteins for each row
    begin
        delete from reaction where reaction.uniprot_id=old.uniprot_id;
    end $$
DELIMITER ;

delimiter $$
CREATE TRIGGER delete_article before DELETE ON reaction
  FOR EACH ROW BEGIN
    set @temp = (select count(*) from reaction where doi =old.doi);
    if @temp = 1 then
        delete from contributors where doi in (select doi from reaction where reaction_id=old.reaction_id);
        delete from article where doi in (select doi from reaction where reaction_id=old.reaction_id);
    end if;
    end;$$
delimiter ;

DELIMITER $$
create trigger trigger_drug_delete before delete on drugs for each row
    begin
        delete from reaction where reaction.drugbank_id=old.drugbank_id;
        delete from has where has.drugbank_id=OLD.drugbank_id;
        delete from interactions where drug1=old.drugbank_id;
        delete from interactions where drug2=old.drugbank_id;
    end $$
DELIMITER ;

create
    definer = root@localhost procedure filter(IN p_drugbank_id text, IN p_meas_type text, IN p_low double,
                                              IN p_high double)
BEGIN
    select r.uniprot_id, r.target_name from reaction r
    where r.drugbank_id like p_drugbank_id and p_meas_type like r.measure and r.affinity_nM between p_low and p_high;
    
end;

CREATE TRIGGER limit_dbmanager
BEFORE INSERT ON databasemanager
FOR EACH ROW
BEGIN
    DECLARE cnt INT;

    SELECT count(*) INTO cnt FROM databasemanager;

    IF cnt = 5 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'You can store only 5 records.';
    END IF;
END;




drop table bindingdb;
drop table drugbank;
drop table sider;





