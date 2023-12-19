USE AccountDB;

SET NOCOUNT ON
GO

SELECT * 
FROM INFORMATION_SCHEMA.KEY_COLUMN_USAGE
WHERE TABLE_NAME = 'Account'

IF EXISTS (	SELECT * FROM INFORMATION_SCHEMA.TABLES 
			WHERE TABLE_NAME = 'Account' AND TABLE_SCHEMA = 'dbo' )
	DROP TABLE Account;
GO

CREATE TABLE Account (
	accountId	INT				NOT NULL PRIMARY KEY IDENTITY(1,1),
	accountName NVARCHAR(64)	NOT NULL DEFAULT (N'unkonwn'),
	password	VARBINARY(MAX)	NOT NULL
);
GO

CREATE UNIQUE INDEX Account_uniqueIndex_accountName ON Account(accountName);


INSERT INTO Account
VALUES (N'Test')

 IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_SelectAccountId'))
	DROP PROCEDURE usp_SelectAccountId
GO

CREATE PROCEDURE usp_SelectAccountId
	@name	NVARCHAR(64)
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
	@name		NVARCHAR(64),
	@password	NVARCHAR(64)
AS
BEGIN
	DECLARE @salt VARBINARY(4) = CRYPT_GEN_RANDOM(4);
	DECLARE @hash VARBINARY(MAX); 
	SET @hash = 0x0200 + @salt + HASHBYTES('SHA2_512', CAST(@password AS VARBINARY(MAX)) + @salt);
	--SELECT @hash AS HashValue, PWDCOMPARE(@pswd,@hash) AS IsPasswordHash;

	SET NOCOUNT ON
	INSERT INTO Account
	VALUES(@name, @hash)
	SELECT @@IDENTITY
END
GO

IF EXISTS ( SELECT * FROM sys.objects 
			WHERE type = 'P' AND OBJECT_ID = OBJECT_ID('usp_LoginAccount'))
	DROP PROCEDURE usp_LoginAccount
GO

CREATE PROCEDURE usp_LoginAccount
	@name		NVARCHAR(64),
	@password	NVARCHAR(64)
AS
BEGIN
	DECLARE @findAccountId AS INT;

	SET @findAccountId = (SELECT accountId	
						FROM Account												
						WHERE accountName = @name)

	IF @findAccountId IS NOT NULL
	BEGIN
		DECLARE @hash VARBINARY(MAX); 
		DECLARE @isPasswordHash INT; 
		SET @hash =  (SELECT password	
					FROM Account					
					WHERE accountName = @name);
		SET @isPasswordHash = PWDCOMPARE(@password,@hash);

		IF @isPasswordHash IS NOT NULL
			SELECT @findAccountId;
		--	패스워드 불일치
		ELSE
			SELECT 0;
	END
	--	계정 존재하지 않음
	ELSE
		SELECT -1
END
GO


DBCC CHECKIDENT(Account, reseed, 0);
GO

EXEC usp_CreateAccount N'홍길33순',N'tesfs3232'
GO

EXEC usp_LoginAccount N'홍길33순',N'tesfs3232'
GO

SELECT *
FROM Account;
GO

DECLARE @hash VARBINARY(MAX) = 
	(SELECT password
		FROM Account
		WHERE accountId = 1);

SELECT @hash AS HashValue
SELECT PWDCOMPARE(N'tesfs3232',@hash) AS IsPasswordHash
GO

DELETE FROM Account;
GO
