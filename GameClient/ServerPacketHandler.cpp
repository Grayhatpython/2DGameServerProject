#include "pch.h"
#include "ServerPacketHandler.h"

#include "Ball.h"
#include "Player.h"
#include "Monster.h"
#include "MyPlayer.h"

#include "FollowCamera.h"
#include "ActorManager.h"
#include "FSMComponent.h"
#include "GUIComponent.h"
#include "MoveComponent.h"
#include "StatComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "AnimSpriteComponent.h"

#include "Hit.h"
#include "BallDestory.h"

#include "Item.h"
#include "GUIInventory.h"
#include "InventoryManager.h"

#include "HealthBar.h"
#include "GUIChatBox.h"
#include "GUIManager.h"
#include "GUILoginForm.h"
#include "GUIServerSelectionForm.h"

#include "MapManager.h"
#include "DataManager.h"
#include "NetworkManager.h"

#include "PlayGameScene.h"
#include "GameSceneManager.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	auto packetHeader = reinterpret_cast<PacketHeader*>(buffer);

	return false;
}

bool S_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CONNECTED& packet)
{
	std::wcout << L"S_CONNECTED_Packet_Processing_Function" << std::endl;

	Protocol::C_LOGIN loginSendPacket;

	//	유니크 ID .. 고민중
	loginSendPacket.set_uniqueid("csi");
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginSendPacket);
	GNetworkManager->Send(loginSendPacket);

	return true;
}

bool S_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet)
{
	std::wcout << L"S_LOGIN_Packet_Processing_Function" << std::endl;
	std::wcout << packet.loginok() << std::endl;

	//	로비 UI and 캐릭터 선택
	
	if (packet.lobbyplayers().size() == 0)
	{
		Protocol::C_CREATE_PLAYER createPlayerSendPacket;
		std::string name = "Player_" + std::to_string(Random::GetIntRange(0, 5000));
		createPlayerSendPacket.set_name(name);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createPlayerSendPacket);
		GNetworkManager->Send(createPlayerSendPacket);
	}
	//	기존의 캐릭터가 있을 시
	else
	{
		//	일단 첫번째 캐릭터로 로그인
		Protocol::LobbyPlayerInfo lobbyPlayerInfo;
		lobbyPlayerInfo = packet.lobbyplayers()[0];
		Protocol::C_ENTER_GAME enterGameSendPacket;
		enterGameSendPacket.set_name(lobbyPlayerInfo.name());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGameSendPacket);
		GNetworkManager->Send(enterGameSendPacket);
	}
	return true;
}

bool S_CREATE_PLAYER_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CREATE_PLAYER& packet)
{
	std::wcout << L"S_CREATE_PLAYER_Packet_Processing_Function" << std::endl;

	//	캐릭터 만들기 성공 시
	if (packet.createok())
	{
		//	해당 캐릭터로 게임 접속 패킷 전달
		Protocol::C_ENTER_GAME enterGameSendPacket;
		enterGameSendPacket.set_name(packet.createplayer().name());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGameSendPacket);
		GNetworkManager->Send(enterGameSendPacket);
	}
	//	실패 시 다시 만들어달라고 요청
	else
	{
		Protocol::C_CREATE_PLAYER createPlayerSendPacket;
		std::string name = "Player_" + std::to_string(Random::GetIntRange(0, 5000));
		createPlayerSendPacket.set_name(name);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createPlayerSendPacket);
		GNetworkManager->Send(createPlayerSendPacket);
	}

	return true;
}

bool S_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& packet)
{
	std::wcout << L"S_ENTER_GAME_Packet_Processing_Function" << std::endl;
	{
		int32 id = static_cast<int32>(packet.player().id());
		auto myPlayer = GActorManager->AddActor<MyPlayer>(packet.player(), id);

		auto stat = myPlayer->GetComponent<StatComponent>();
		stat->SetStatInfo(packet.player().statinfo());
		stat->ReCalculationAdditionalStat();

		auto gui = myPlayer->GetComponent<GUIComponent>();
		auto healthBar = static_cast<HealthBar*>(gui->FindGuiFromName(L"HealthBar"));
		healthBar->SetOwner(myPlayer);
		healthBar->SetState(GUIState::Active);
		healthBar->SetHealthBarRatio();

		auto fsm = myPlayer->GetComponent<FSMComponent>();
		ASSERT(fsm);
		fsm->ChangeState(packet.player().positioninfo().state());

		GActorManager->SetMyPlayer(std::static_pointer_cast<Actor>(myPlayer));

		auto camera = std::make_shared<FollowCamera>();
		GActorManager->SetCamera(camera);
		camera->SetName(L"Camera");

		camera->AddComponent<CameraComponent>()->SetTarget(myPlayer);
	}

	return true;
}

bool S_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& packet)
{
	std::wcout << L"S_LEAVE_GAME_Packet_Processing_Function" << std::endl;
	//	TODO?
	GActorManager->RemoveActors();

	return true;
}

bool S_ITEM_LIST_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ITEM_LIST& packet)
{
	std::wcout << L"S_ITEM_LIST_Packet_Processing_Function" << std::endl;

	auto guiInventory = GGUIManager->GetGui(L"Inventory");
	ASSERT(guiInventory);

	GInventoryManager->Clear();

	for (auto& itemInfo : packet.iteminfos())
	{
		auto newItem = Item::MakeItem(itemInfo);
		GInventoryManager->AddItem(newItem);
	}

	return true;
}

bool S_ADD_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ADD_ITEM& packet)
{
	std::wcout << L"S_ADD_ITEM_Packet_Processing_Function" << std::endl;

	auto guiInventory = GGUIManager->GetGui(L"Inventory");
	ASSERT(guiInventory);

	for (auto& itemInfo : packet.iteminfos())
	{
		auto newItem = Item::MakeItem(itemInfo);
		GInventoryManager->AddItem(newItem);
	}

	//	TEMP
	std::wcout << L"아이템 획득" << std::endl;

	if(guiInventory->GetState() == GUIState::Active)
		static_cast<GUIInventory*>(guiInventory)->Refresh();

	auto myPlayer = GActorManager->GetMyPlayer();
	ASSERT(myPlayer);

	auto stat = myPlayer->GetComponent<StatComponent>();
	stat->ReCalculationAdditionalStat();

	return true;
}

bool S_EQUIP_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_EQUIP_ITEM& packet)
{
	std::wcout << L"S_EQUIP_ITEM_Packet_Processing_Function" << std::endl;
	auto guiInventory = GGUIManager->GetGui(L"Inventory");
	ASSERT(guiInventory);

	auto item = GInventoryManager->GetItem(packet.itemdbid());
	ASSERT(item);

	//	TEMP
	std::wcout << L"아이템 장착 여부 변경" << std::endl;

	item->SetEquipped(packet.equipped());

	if (guiInventory->GetState() == GUIState::Active)
		static_cast<GUIInventory*>(guiInventory)->Refresh();

	auto myPlayer = GActorManager->GetMyPlayer();
	ASSERT(myPlayer);

	auto stat = myPlayer->GetComponent<StatComponent>();
	stat->ReCalculationAdditionalStat();

	return true;
}

bool S_SPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SPAWN& packet)
{
	std::wcout << L"S_SPAWN_Packet_Processing_Function" << std::endl;
	
	if (packet.actors().size() > 0)
	{
		for (auto& actorInfo : packet.actors())
		{
			if (GActorManager->FindActor(static_cast<int32>(actorInfo.id())))
				continue;
			//std::wcout << actor.id() << std::endl;

			auto type = ActorManager::GetActorTypeFromId(static_cast<int32>(actorInfo.id()));

			if (type == Protocol::ActorType::PLAYER)
			{
				int32 id = static_cast<int32>(actorInfo.id());
				auto player = GActorManager->AddActor<Player>(actorInfo, id);

				auto stat = player->GetComponent<StatComponent>();
				stat->SetStatInfo(actorInfo.statinfo());

				auto gui = player->GetComponent<GUIComponent>();
				auto healthBar = static_cast<HealthBar*>(gui->FindGuiFromName(L"HealthBar"));
				healthBar->SetOwner(player);
				healthBar->SetState(GUIState::Active);
				healthBar->SetHealthBarRatio();

				auto fsm = player->GetComponent<FSMComponent>();
				ASSERT(fsm);		

				if (actorInfo.positioninfo().state() == Protocol::AIState::Skill)
				{
					const auto& skillData = DataManager::FindSkillData(actorInfo.positioninfo().usedskillid());
					auto skillName = skillData.name;

					fsm->ChangeState(skillName);
				}
				else
					fsm->ChangeState(actorInfo.positioninfo().state());


			}
			else if (type == Protocol::ActorType::MONSTER)
			{
				int32 id = static_cast<int32>(actorInfo.id());
				auto monster = GActorManager->AddActor<Monster>(actorInfo ,id);

				auto stat = monster->GetComponent<StatComponent>();
				stat->SetStatInfo(actorInfo.statinfo());

				auto gui = monster->GetComponent<GUIComponent>();
				auto healthBar = static_cast<HealthBar*>(gui->FindGuiFromName(L"HealthBar"));
				healthBar->SetOwner(monster);
				healthBar->SetState(GUIState::Active);
				healthBar->SetHealthBarRatio();

				auto fsm = monster->GetComponent<FSMComponent>();
				ASSERT(fsm);

				if (actorInfo.positioninfo().state() == Protocol::AIState::Skill)
				{
					const auto& skillData = DataManager::FindSkillData(actorInfo.positioninfo().usedskillid());
					auto skillName = skillData.name;

					fsm->ChangeState(skillName);
				}
				else
					fsm->ChangeState(actorInfo.positioninfo().state());
			}
			else if (type == Protocol::ActorType::PROJECTILE)
			{
				int32 id = static_cast<int32>(actorInfo.id());
				auto ball = GActorManager->AddActor<Ball>(actorInfo, id);

				auto stat = ball->GetComponent<StatComponent>();
				stat->SetStatInfo(actorInfo.statinfo());

				auto fsm = ball->GetComponent<FSMComponent>();
				ASSERT(fsm);
				fsm->ChangeState(actorInfo.positioninfo().state());
			}
		}
	}

	return true;
}

bool S_DESPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& packet)
{
	std::cout << "S_DESPAWN_Packet_Processing_Function" << std::endl;

	if (packet.actorid().size() > 0)
	{
		for (auto& id : packet.actorid())
		{
			auto actor = GActorManager->FindActor(id);
			if (actor == nullptr)
				continue;

			GActorManager->RemoveActor(id);

			if (ActorManager::GetActorTypeFromId(id) == Protocol::ActorType::PROJECTILE)
			{
				auto transform = actor->GetComponent<TransformComponent>();
				ASSERT(transform);

				auto move = actor->GetComponent<MoveComponent>();
				ASSERT(move);

				if (GMapManager->CanMovePosition(transform->GetForwardPosition(move->GetMoveDir())))
					return true;

				auto ballDestory = GActorManager->AddEffect<BallDestory>();

				auto ballDestoryTransform = ballDestory->GetComponent<TransformComponent>();
				ASSERT(ballDestoryTransform);

				auto actorTransform = actor->GetComponent<TransformComponent>();
				ASSERT(actorTransform);
				
				ballDestoryTransform->SetRotation(actorTransform->GetRotation());
				ballDestoryTransform->SetPosition(actorTransform->GetPosition());

				auto ballDestorySprite = ballDestory->GetComponent<AnimSpriteComponent>();
				ASSERT(ballDestorySprite);
				ballDestorySprite->Play(L"FIREBALL_DESTORY_EFFECT");
			}

		}
	}

	return true;
}

bool S_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_MOVE& packet)
{
	std::cout << "S_MOVE_Packet_Processing_Function" << std::endl;

	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;

	//if (GActorManager->GetMyPlayer()->GetId() == packet.actorid())
	//	return true;

	auto move = actor->GetComponent<MoveComponent>();
	ASSERT(move);
	auto transform = actor->GetComponent<TransformComponent>();
	ASSERT(transform);
	auto fsm = actor->GetComponent<FSMComponent>();
	ASSERT(fsm);

	Vector2 position{};
	position.x = packet.positioninfo().positionx();	
	position.y = packet.positioninfo().positiony();
	move->SetMoveDir(packet.positioninfo().movedir());
	move->SetMovePosition(position);
	fsm->ChangeState(packet.positioninfo().state());

	return true;
}

bool S_SKILL_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SKILL& packet)
{
	std::cout << "S_SKILL_Packet_Processing_Function" << std::endl;

	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;

	const auto& skillData = DataManager::FindSkillData(packet.skillinfo().skillid());
	auto skillName = skillData.name;

	auto fsm = actor->GetComponent<FSMComponent>();
	ASSERT(fsm);
	fsm->ChangeState(skillName);

	return true;
}

bool S_CHANGE_HP_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_HP& packet)
{
	std::cout << "S_CHANGE_HP_Packet_Processing_Function" << std::endl;

	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;


	auto stat = actor->GetComponent<StatComponent>();
	ASSERT(stat);

	auto gui = actor->GetComponent<GUIComponent>();
	ASSERT(gui);


	{
		//	TEMP;;

		stat->SetHp(packet.hp());

		/*
		auto fsm = actor->GetComponent<FSMComponent>();
		ASSERT(fsm);

		fsm->ChangeState(L"Damaged");
		*/

		auto healthBar = static_cast<HealthBar*>(gui->FindGuiFromName(L"HealthBar"));
		ASSERT(healthBar);
		healthBar->SetHealthBarRatio();

		if (packet.hp() > 0)
		{
			auto hit = GActorManager->AddEffect<Hit>();

			auto hitTransform = hit->GetComponent<TransformComponent>();
			ASSERT(hitTransform);

			auto actorTransfrom = actor->GetComponent<TransformComponent>();
			ASSERT(actorTransfrom);

			auto sprite = hit->GetComponent<AnimSpriteComponent>();
			ASSERT(sprite);

			sprite->Play(L"TARGET_HIT_EFFECT");
			hitTransform->SetPosition(actorTransfrom->GetPosition());
		}

		std::wcout << "HP : " << packet.hp() << std::endl;

		if (actor == GActorManager->GetMyPlayer())
		{
			auto guiInventory = GGUIManager->GetGui(L"Inventory");
			ASSERT(guiInventory);
			static_cast<GUIInventory*>(guiInventory)->RefreshHp();
		}
	}

	return true;
}

bool S_CHANGE_STAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_STAT& packet)
{
	std::cout << "S_CHANGE_STAT_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_DEATH_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DEATH& packet)
{
	std::cout << "S_DEATH_Packet_Processing_Function" << std::endl;

	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;

	auto fsm = actor->GetComponent<FSMComponent>();
	ASSERT(fsm);

	fsm->ChangeState(Protocol::AIState::DEATH);

	return true;
}

bool S_CHANGE_STATE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_STATE& packet)
{
	std::cout << "S_CHANGE_STATE_Packet_Processing_Function" << std::endl;

	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;

	auto fsm = actor->GetComponent<FSMComponent>();
	ASSERT(fsm);

	fsm->ChangeState(packet.state());

	return true;
}

bool S_CHANGE_MOVE_DIR_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_MOVE_DIR& packet)
{
	std::cout << "S_CHANGE_MOVE_DIR_Packet_Processing_Function" << std::endl;

	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;

	auto move = actor->GetComponent<MoveComponent>();
	ASSERT(move);
	move->SetMoveDir(packet.movedir());
	return true;
}

bool S_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& packet)
{
	auto actor = GActorManager->FindActor(packet.actorid());
	//	Zone 이슈
	if (actor == nullptr)
		return true;

	auto gui = GGUIManager->GetGui(L"ChatBox");
	ASSERT(gui);

	auto chatBox = static_cast<GUIChatBox*>(gui);
	auto chatMessage = UtilityHelper::ConvertUTF8ToUnicode(packet.chat());
	chatBox->AddChatMessage(actor->GetName(), chatMessage);

	return true;
}

bool S_PING_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_PING& packet)
{
	std::cout << "S_PING_Packet_Processing_Function" << std::endl;

	Protocol::C_PONG pongSendPacket;
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pongSendPacket);
	GNetworkManager->Send(sendBuffer);
	return true;
}

//	TODO
bool L_S_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_CONNECTED& packet)
{
	std::cout << "L_S_CONNECTED_Packet_Processing_Function" << std::endl;


	//	Login Server -> Name Password Send
	/*Protocol::L_C_CREATE_ACCOUNT createAccountSendPacket;
	createAccountSendPacket.set_name("prouser");
	createAccountSendPacket.set_password("123456");
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createAccountSendPacket);
	GNetworkManager->Send(sendBuffer);*/

	return true;
}

bool L_S_CREATE_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_CREATE_ACCOUNT& packet)
{
	std::cout << "L_S_CREATE_ACCOUNT_Packet_Processing_Function" << std::endl;

	if (packet.createok())
	{
		std::wcout << L"계정 생성 성공" << std::endl;
		auto gui = GGUIManager->GetGui(L"LoginForm");
		ASSERT(gui);
		auto loginForm = static_cast<GUILoginForm*>(gui);
		ASSERT(loginForm);
		loginForm->Clear();
	}
	//	실패 ? 사유가 멀까.. 패킷으로 보내주자
	else
	{

	}

	return true;
}

bool L_S_LOGIN_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_LOGIN_ACCOUNT& packet)
{
	std::cout << "L_S_CREATE_ACCOUNT_Packet_Processing_Function" << std::endl;

	//	게임 씬으로..
	if (packet.loginok())
	{
		auto loginForm = GGUIManager->GetGui(L"LoginForm");
		ASSERT(loginForm);
		loginForm->SetState(GUIState::DeActive);

		auto serverSelectionForm = GGUIManager->GetGui(L"ServerSelectionForm");
		ASSERT(serverSelectionForm);

		std::vector<Protocol::ServerInfo> serverInfos;
		for (auto serverInfo : packet.serverinfos())
			serverInfos.push_back(serverInfo);

		static_cast<GUIServerSelectionForm*>(serverSelectionForm)->SetServerInfos(serverInfos);

		serverSelectionForm->SetState(GUIState::Active);

		{
			//	로그인 서버 연결 종료
			GNetworkManager->SetClientGameState(Protocol::ClientGameState::CLIENT_STATE_GAME);
			session->Disconnect(L"로그인 성공 -> 로그인 서버 연결 종료");
			std::this_thread::sleep_for(100ms);
		}
	}
	//	실패 ? 사유가 멀까.. 패킷으로 보내주자
	else
	{
		
	}

	return true;
}