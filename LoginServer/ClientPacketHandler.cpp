#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "DBHelper.h"
#include "DBConnectionPool.h"
#include "Random.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool L_C_CREATE_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_C_CREATE_ACCOUNT& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);

	auto dbConnection = GDBConnectionPool->Pop(L"AccountDB");
	dbConnection->UnBind();

	DBHelper<1, 1> dbHepler(dbConnection, L"EXEC usp_SelectAccountId ?");
	auto name = UtilityHelper::ConvertUTF8ToUnicode(packet.name());
	dbHepler.BindParam(0, name.c_str());

	int32 accountId = 0;
	dbHepler.BindCol(0, accountId);
	ASSERT(dbHepler.Execute());

	while (dbHepler.Fetch())
		accountId;

	GDBConnectionPool->Push(L"AccountDB", dbConnection);

	Protocol::L_S_CREATE_ACCOUNT createAccountSendPacket;
	
	if (accountId)
		//	�̹� ����
		createAccountSendPacket.set_createok(false);
	else
	{
		auto dbConnection = GDBConnectionPool->Pop(L"AccountDB");
		dbConnection->UnBind();

		DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_CreateAccount ?, ?");
		auto name = UtilityHelper::ConvertUTF8ToUnicode(packet.name());
		auto password = UtilityHelper::ConvertUTF8ToUnicode(packet.password());
		dbHepler.BindParam(0, name.c_str());
		dbHepler.BindParam(1, password.c_str());

		int32 newAccountId = 0;
		dbHepler.BindCol(0, newAccountId);
		ASSERT(dbHepler.Execute());

		while (dbHepler.Fetch())
			newAccountId;

		GDBConnectionPool->Push(L"AccountDB", dbConnection);

		if (newAccountId)
			//	����
			createAccountSendPacket.set_createok(true);
		else
			//	���ÿ� �Ȱ��� �̸��� �����Ϸ��ٰ� �Ѹ��� �����Ѱ��
			createAccountSendPacket.set_createok(false);
			//	����..
	}
	
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(createAccountSendPacket);
	clientSession->Send(sendBuffer);

	return true;
}

bool L_C_LOGIN_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_C_LOGIN_ACCOUNT& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);

	auto dbConnection = GDBConnectionPool->Pop(L"AccountDB");
	dbConnection->UnBind();

	DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_LoginAccount ?, ?");
	auto name = UtilityHelper::ConvertUTF8ToUnicode(packet.name());
	auto password = UtilityHelper::ConvertUTF8ToUnicode(packet.password());
	dbHepler.BindParam(0, name.c_str());
	dbHepler.BindParam(1, password.c_str());

	int32 accountId = 0;
	dbHepler.BindCol(0, accountId);
	ASSERT(dbHepler.Execute());

	while (dbHepler.Fetch())
		accountId;

	GDBConnectionPool->Push(L"AccountDB", dbConnection);

	Protocol::L_S_LOGIN_ACCOUNT loginAccountSendPacket;

	if (accountId > 0)
	{
		//	�α��� ���� -> ���� 
		auto dbConnection = GDBConnectionPool->Pop(L"MasterDB");
		dbConnection->UnBind();

		DBHelper<1, 1> dbHepler(dbConnection, L"EXEC usp_SelectAuthId ?");
		dbHepler.BindParam(0, accountId);

		int32 authId = 0;
		dbHepler.BindCol(0, authId);
		ASSERT(dbHepler.Execute());

		while (dbHepler.Fetch())
			authId;

		int32 token = Random::GetIntRange(INT32_MIN, INT32_MAX);

		// ���� ���� ( ��ū , ��ȿ�Ⱓ ) ������Ʈ
		if (authId > 0)
		{
			dbConnection->UnBind();

			DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_UpdateAuth ?, ?");
			dbHepler.BindParam(0, accountId);
			dbHepler.BindParam(1, token);

			int32 success = 0;
			dbHepler.BindCol(0, authId);
			ASSERT(dbHepler.Execute());

			while (dbHepler.Fetch())
				authId;

			if (authId > 0)
			{
				;
			}
			//	���� ���� ������Ʈ ����?
			else
			{
				;
			}
		}
		//	���� ���� ( ��ū , ��ȿ�Ⱓ ) ����
		else
		{
			dbConnection->UnBind();

			DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_CreateAuth ?, ?");
			dbHepler.BindParam(0, accountId);
			dbHepler.BindParam(1, token);

			int32 success = 0;
			dbHepler.BindCol(0, success);
			ASSERT(dbHepler.Execute());

			while (dbHepler.Fetch())
				success;

			if (success > 0)
			{
				;
			}
			//	���� ���� ���� ����?
			else
			{
				;
			}
		}

		loginAccountSendPacket.set_loginok(true);
		loginAccountSendPacket.set_accountid(accountId);
		loginAccountSendPacket.set_token(token);

		//	���� ���� ���� ����� �����ش�..
		{
			dbConnection->UnBind();

			DBHelper<0, 4> dbHepler(dbConnection, L"EXEC usp_SelectGameServer");
			WCHAR serverName[64] = { 0, };
			WCHAR serverIp[64] = { 0, };
			int32 port = 0;;
			int32 complexity = 0;
			dbHepler.BindCol(0, serverName);
			dbHepler.BindCol(1, serverIp);
			dbHepler.BindCol(2, port);
			dbHepler.BindCol(3, complexity);
			ASSERT(dbHepler.Execute());

			//	������� ����.. ���Ӽ������� ����� �ʿ��ҰŰ���..
			//	Or MasterDB�� GameServer�� ������ ���� ���⵵�� ������Ʈ �ϸ� �α��� ���������� MasterDB�� �ֱ������� ������Ʈ�Ѵ�.
			while (dbHepler.Fetch())
			{
				auto serverInfo = loginAccountSendPacket.add_serverinfos();
				serverInfo->set_name(UtilityHelper::ConvertUnicodeToUTF8(serverName));
				serverInfo->set_ip(UtilityHelper::ConvertUnicodeToUTF8(serverIp));
				serverInfo->set_port(port);
				serverInfo->set_complexity(complexity);
			}
		}

		GDBConnectionPool->Push(L"MasterDB", dbConnection);
	}

	else if (accountId == 0)
	{
		//	�н����尡 ����ġ
		loginAccountSendPacket.set_loginok(false);
		loginAccountSendPacket.set_loginfailedreason("Login information mismatch!");
	}
	else
	{
		//	�ش� ���� ����
		loginAccountSendPacket.set_loginok(false);
		loginAccountSendPacket.set_loginfailedreason("Login account does not exist!");
	}


	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginAccountSendPacket);
	clientSession->Send(sendBuffer);
	return true;
}
