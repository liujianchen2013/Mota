--创建map_data表
CREATE TABLE IF NOT EXISTS map_data(
    tier INTEGER PRIMARY KEY NOT NULL,
    enemy TEXT, 
    item TEXT,
    door TEXT,
    npc TEXT,
    teleport TEXT);

--创建hero_data表
CREATE TABLE IF NOT EXISTS hero_data(
    id INTEGER PRIMARY KEY NOT NULL,
    attack INTEGER NOT NULL, 
	defense INTEGER NOT NULL,
    life INTEGER NOT NULL,
    yellowKey INTEGER NOT NULL,
    blueKey INTEGER NOT NULL,
    redKey INTEGER NOT NULL,
    curTier INTEGER NOT NULL,
    other TEXT);