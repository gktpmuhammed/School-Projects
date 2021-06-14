DROP TABLE Reaction;
DROP TABLE Has;
DROP TABLE Article;
DROP TABLE Drug;
DROP TABLE Side_effect;
DROP TABLE Proteins;
DROP TABLE User;
DROP TABLE Database_manager;
--Firstly we dropped relationships. Reaction and has. Then we dropped Article because there was a relation between article and user and we have username and institution in Article. After that we can drop Drug, Side_effect, Proteins
-- and User no matter the order. Lastly we dropped Database_manager.