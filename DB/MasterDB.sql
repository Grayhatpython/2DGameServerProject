USE MasterDB;

SET NOCOUNT ON
GO

SELECT * FROM INFORMATION_SCHEMA.TABLES;

SELECT * 
FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE

IF EXISTS (	SELECT * FROM INFORMATION_SCHEMA.TABLES 
			WHERE TABLE_NAME = 'Auth' AND TABLE_SCHEMA = 'dbo' )
	DROP TABLE Auth;
GO

CREATE TABLE Auth (
	authId			INT NOT NULL PRIMARY KEY IDENTITY(1,1),
	accountId		INT NOT NULL,
	token			INT NOT NULL,
	expiration		DATETIME NOT NULL
);
GO

IF EXISTS (	SELECT * FROM INFORMATION_SCHEMA.TABLES 
			WHERE TABLE_NAME = 'GameServer' AND TABLE_SCHEMA = 'dbo' )
	DROP TABLE GameServer;
GO

CREATE TABLE GameServer (
	serverId		INT NOT NULL PRIMARY KEY IDENTITY(1,1),
	serverName		NVARCHAR(64) NOT NULL DEFAULT (N'unkonwn'),
	serverIp		NVARCHAR(64) NOT NULL DEFAULT (N'unkonwn'),
	serverPort		INT NOT NULL,
	complexity		INT DEFAULT (0),
);
GO

CREATE UNIQUE INDEX Auth_uniqueIndex_accountId ON Auth(accountId);
CREATE UNIQUE INDEX GameServer_uniqueIndex_serverName ON GameServer(serverName);
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectAuthId'))
	DROP PROCEDURE usp_SelectAuthId
GO

CREATE PROCEDURE usp_SelectAuthId
	@accountId	INT
AS
BEGIN
	DECLARE @findAuthId AS INT;

	SET @findAuthId = (SELECT authId	
						FROM Auth												
						WHERE accountId = @accountId)

	IF @findAuthId IS NOT NULL
		SELECT @findAuthId
	ELSE
		SELECT 0
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_UpdateAuth'))
	DROP PROCEDURE usp_UpdateAuth
GO

CREATE PROCEDURE usp_UpdateAuth
	@accountId		INT,
	@token			INT
AS
BEGIN
	DECLARE @findAuthId AS INT;

	SET @findAuthId = (SELECT authId	
						FROM Auth												
						WHERE accountId = @accountId)

	IF @findAuthId IS NOT NULL
	BEGIN
		DECLARE @expiration DATETIME
		SET @expiration = DATEADD(minute, 10, GETUTCDATE());

		SET NOCOUNT ON
		UPDATE Auth
		SET token = @token, expiration = @expiration
		WHERE accountId = @accountId
		SELECT @@ROWCOUNT
	END
	ELSE
		SELECT 0
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_CreateAuth'))
	DROP PROCEDURE usp_CreateAuth
GO

CREATE PROCEDURE usp_CreateAuth
	@accountId		INT,
	@token			INT
AS
BEGIN
	DECLARE @expiration DATETIME
	SET @expiration = DATEADD(minute, 10, GETUTCDATE());
	SET NOCOUNT ON
	INSERT INTO Auth
	VALUES(@accountId, @token, @expiration)
	SELECT @@IDENTITY
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectGameServer'))
	DROP PROCEDURE usp_SelectGameServer
GO

CREATE PROCEDURE usp_SelectGameServer
AS
BEGIN
	SELECT serverName, serverIp, serverPort, complexity
	FROM GameServer;
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectGameServerId'))
	DROP PROCEDURE usp_SelectGameServerId
GO

CREATE PROCEDURE usp_SelectGameServerId
	@name		NVARCHAR(64)
AS
BEGIN
	DECLARE @findGameServerId AS INT;

	SET @findGameServerId = (SELECT serverId	
						FROM GameServer												
						WHERE serverName = @name)

	IF @findGameServerId IS NOT NULL
		SELECT @findGameServerId
	ELSE
		SELECT 0;
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_CreateGameServer'))
	DROP PROCEDURE usp_CreateGameServer
GO

CREATE PROCEDURE usp_CreateGameServer
	@name			NVARCHAR(64),
	@ip				NVARCHAR(64),
	@port			INT,
	@complexity		INT
AS
BEGIN
	SET NOCOUNT ON
	INSERT INTO GameServer
	VALUES(@name, @ip, @port, @complexity)
	SELECT @@IDENTITY
END
GO

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_UpdateGameServer'))
	DROP PROCEDURE usp_UpdateGameServer
GO

CREATE PROCEDURE usp_UpdateGameServer
	@name			NVARCHAR(64),
	@ip				NVARCHAR(64),
	@port			INT,
	@complexity		INT
AS
BEGIN
	DECLARE @findGameServerId AS INT;

	SET @findGameServerId = (SELECT serverId	
						FROM GameServer												
						WHERE serverName = @name)

	IF @findGameServerId IS NOT NULL
	BEGIN
		SET NOCOUNT ON
		UPDATE GameServer
		SET serverName = @name, serverIp = @ip, serverPort = @port, complexity = @complexity
		WHERE serverName = @name
		SELECT @@ROWCOUNT
	END
	ELSE
		SELECT 0
END
GO

DBCC CHECKIDENT(Auth, reseed, 0);
DBCC CHECKIDENT(GameServer, reseed, 0);
GO

SELECT GETUTCDATE();

SELECT *
FROM Auth;

SELECT *
FROM GameServer;