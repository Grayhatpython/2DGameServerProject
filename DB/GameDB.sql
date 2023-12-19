USE GameServerDB;

SET NOCOUNT ON
GO

SELECT * FROM INFORMATION_SCHEMA.TABLES;

SELECT * 
FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE
WHERE TABLE_NAME = 'Player'

IF EXISTS (	SELECT * FROM INFORMATION_SCHEMA.TABLES 
			WHERE TABLE_NAME = 'Account' AND TABLE_SCHEMA = 'dbo' )
	DROP TABLE Account;
GO

IF EXISTS (	SELECT * FROM INFORMATION_SCHEMA.TABLES 
			WHERE TABLE_NAME = 'Player' AND TABLE_SCHEMA = 'dbo' )
	DROP TABLE Player;
GO

IF EXISTS (	SELECT * FROM INFORMATION_SCHEMA.TABLES 
			WHERE TABLE_NAME = 'Item' AND TABLE_SCHEMA = 'dbo' )
	DROP TABLE Item;
GO

CREATE TABLE Account (
	accountId		INT NOT NULL PRIMARY KEY IDENTITY(1,1),
	accountName		NVARCHAR(64) NOT NULL DEFAULT (N'unkonwn')
);
GO

CREATE TABLE Player (
	playerId		INT NOT NULL PRIMARY KEY IDENTITY(1,1),
	playerName		NVARCHAR(64) NOT NULL DEFAULT (N'unkonwn'),
	level			INT,
	hp				INT,
	maxHp			INT,
	attack			INT,
	speed			FLOAT,
	totalExp		INT,
	accountId		INT NOT NULL,
	CONSTRAINT		FK_acoountId_Account FOREIGN KEY (accountId) REFERENCES Account(accountId)
);
GO

CREATE TABLE Item (
	itemId			INT NOT NULL PRIMARY KEY IDENTITY(1,1),
	dataSheetId		INT NOT NULL,
	count			INT,
	slot			INT,
	equipped		BIT DEFAULT 0,
	playerId		INT,
	CONSTRAINT		FK_playerId_Player FOREIGN KEY (playerId) REFERENCES Player(playerId)
);
GO

CREATE UNIQUE INDEX Account_uniqueIndex_accountName ON Account(accountName);
CREATE UNIQUE INDEX Player_uniqueIndex_playerName ON Player(playerName);
GO


 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectAccountId'))
	DROP PROCEDURE usp_SelectAccountId
GO

CREATE PROCEDURE usp_SelectAccountId
	@name		NVARCHAR(64)
AS
BEGIN
	DECLARE @findAccountId AS INT;

	SET @findAccountId = (SELECT accountId	
						FROM Account												
						WHERE accountName = @name)

	IF @findAccountId IS NOT NULL
		SELECT @findAccountId
	ELSE
		SELECT 0
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_CreateAccount'))
	DROP PROCEDURE usp_CreateAccount
GO

CREATE PROCEDURE usp_CreateAccount
	@name		NVARCHAR(64)
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO Account
	VALUES(@name)
	SELECT @@IDENTITY
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectPlayers'))
	DROP PROCEDURE usp_SelectPlayers
GO

CREATE PROCEDURE usp_SelectPlayers
	@uniqueId	NVARCHAR(64)
AS
BEGIN
	SET NOCOUNT ON
	SELECT
	playerId, level, hp, maxHp, attack, speed, totalExp			
	FROM Player 
	WHERE accountId = (SELECT accountId FROM Account WHERE accountName = @uniqueId)
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectPlayerId'))
	DROP PROCEDURE usp_SelectPlayerId
GO

CREATE PROCEDURE usp_SelectPlayerId
	@name	NVARCHAR(64)
AS
BEGIN
	DECLARE @findPlayerId AS INT;

	SET @findPlayerId = (SELECT playerId	
						FROM Player												
						WHERE playerName = @name)

	IF @findPlayerId IS NOT NULL
		SELECT @findPlayerId
	ELSE
		SELECT 0
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_UpdatePlayerHp'))
	DROP PROCEDURE usp_UpdatePlayerHp
GO

CREATE PROCEDURE usp_UpdatePlayerHp
	@id		INT,
	@hp		INT
AS
BEGIN
	DECLARE @findPlayerId AS INT;

	SET @findPlayerId = (SELECT playerId	
						FROM Player												
						WHERE playerId = @id)

	IF @findPlayerId IS NOT NULL
	BEGIN
		SET NOCOUNT ON
		UPDATE Player
		SET hp = @hp
		WHERE playerId = @id
		SELECT @@ROWCOUNT
	END
	ELSE
		SELECT 0
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_InsertPlayer'))
	DROP PROCEDURE usp_InsertPlayer
GO

CREATE PROCEDURE usp_InsertPlayer
	@name		NVARCHAR(64),
	@level		INT,
	@hp			INT,
	@maxHp		INT,
	@attack		INT,
	@speed		FLOAT,
	@totalExp	INT,
	@accountId	INT
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO Player
	VALUES(@name, @level, @hp, @maxHp, @attack, @speed, @totalExp, @accountId)
	SELECT @@IDENTITY
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectItems'))
	DROP PROCEDURE usp_SelectItems
GO

CREATE PROCEDURE usp_SelectItems
	@id		INT
AS
BEGIN
	SET NOCOUNT ON
	SELECT itemId, dataSheetId, count, slot, equipped
	FROM Item
	WHERE item.playerId = @id;
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_InsertItem'))
	DROP PROCEDURE usp_InsertItem
GO

CREATE PROCEDURE usp_InsertItem
	@dataSheetId	INT,
	@count			INT,
	@slot			INT,
	@playerId		INT
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO Item(dataSheetId, count, slot, playerId)
	VALUES(@dataSheetId, @count, @slot, @playerId)
	SELECT @@IDENTITY
END
GO


 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_UpdateItemEquipped'))
	DROP PROCEDURE usp_UpdateItemEquipped
GO

CREATE PROCEDURE usp_UpdateItemEquipped
	@id			INT,
	@equipped	BIT
AS
BEGIN
	DECLARE @findItemId AS INT;

	SET @findItemId = (SELECT itemId	
						FROM Item												
						WHERE itemId = @id)

	IF @findItemId IS NOT NULL
	BEGIN
		SET NOCOUNT ON
		UPDATE Item
		SET equipped = @equipped
		WHERE itemId = @findItemId
		SELECT @@ROWCOUNT
	END
	ELSE
		SELECT 0
END
GO

SET NOCOUNT ON
INSERT INTO Player
VALUES (N'test', 40, 40, 40, 40, 40.0, 40, 2)
GO

INSERT INTO Item(dataSheetId, count, slot, playerId) VALUES (1,1,1,1)
INSERT INTO Item(dataSheetId, count, slot, playerId) VALUES (100,1,2,1)
INSERT INTO Item(dataSheetId, count, slot, playerId) VALUES (200,10,3,1)
GO

UPDATE Player
SET hp = 200
WHERE hp = 0

EXEC usp_SelectAccountId N'홍길33순'
EXEC usp_SelectPlayers N'홍길3순'
EXEC usp_InsertPlayer N'Player_1', 11, 11, 11, 11, 11.0, 11, 1

DBCC CHECKIDENT(Account, reseed, 0);
DBCC CHECKIDENT(Player, reseed, 0);
DBCC CHECKIDENT(Item, reseed, 0);
GO


--	계정
SELECT *
FROM Account;
GO

--	캐릭터
SELECT *
FROM Player;
GO

--	아이템
SELECT *
FROM Item;
GO


DELETE FROM Account;
DELETE FROM Player;
DELETE FROM Item;
GO
