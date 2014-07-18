#include <vector>
#include <utility>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>

#include "GameServer.h"
#include "AdminCommands.h"
#include "StringUtility.h"
#include "Teleport.h"
#include "SAMPFunctionUtility.h"

#define ADMIN_CMD_BEGIN_ATR(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN_TARGET_REASON, reg) {} virtual void executeATR(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, const std::string& reason)

#define ADMIN_CMD_BEGIN_AT(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN_TARGET, reg) {} virtual void executeAT(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target)

#define ADMIN_CMD_BEGIN_AR(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN_REASON, reg) {} virtual void executeAR(const std::shared_ptr<Account>& admin, const std::string& reason)

#define ADMIN_CMD_BEGIN_A(name, cnname, reqlv, reg) \
class __AdminCmd##name : public AdminCommandBase{ public: __AdminCmd##name() : AdminCommandBase(cnname, reqlv, ADMIN, reg) {} virtual void executeA(const std::shared_ptr<Account>& admin)

#define ADMIN_CMD_END(name) } __AdminCmdIns##name;

#define ADMIN_LOG(admin, target, reason) \
	GameServer::getInstance().datasource.adminOperationLog(admin->getUserID(), target->getUserID(), __FUNCTION__, reason)

std::vector<AdminCommandBase*> g_admin_player_cmd_reg;
std::vector<AdminCommandBase*> g_admin_server_cmd_reg;

using namespace std::placeholders;

// Player

ADMIN_CMD_BEGIN_AT(ViewProfile, "�鿴����", 0, g_admin_player_cmd_reg)
{
	std::stringstream str;
	str << "ID: " << target->getUserID() <<
		"\n��¼��: " << target->getLogName() <<
		"\n��ʾ��: " << target->getNickname() <<
		"\n���п����: " << target->getMoney() <<
		"\n����Ա�ȼ�: " << target->getAdminLevel();
	GameServer::getInstance().dialogmanager.displayMessageDialog(admin,
		STR(target->getLogName() << " ������").c_str(), str.str(), "ȷ��", "");
}
ADMIN_CMD_END(ViewProfile)

ADMIN_CMD_BEGIN_AT(PM, "����", 0, g_admin_player_cmd_reg)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(admin, "��������", "��������Ϣ", "����", "ȡ��", false,
	[admin, target](const std::string& msg) {
		SendClientMessage(admin->getInGameID(), 0xFFFFFFFF, STR("[Message] ��Ϣ���͸� " << target->getLogName() << UID(target) << ": " << msg).c_str());
		SendClientMessage(target->getInGameID(), 0xFFFFFFFF, STR("[Message] ���� " << admin->getLogName() << UID(admin) << " ����Ϣ: " << msg).c_str());
		ADMIN_LOG(admin, target, msg);
	});
}
ADMIN_CMD_END(PM)

ADMIN_CMD_BEGIN_AT(Goto, "���͹�ȥ", 1, g_admin_player_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ���͵��� " << target->getLogName() << UID(target) << " ���.").c_str());
	teleportToPlayer(admin->getInGameID(), target->getInGameID());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Goto)

ADMIN_CMD_BEGIN_AT(Get, "������", 1, g_admin_player_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" �� " << target->getLogName() << UID(target) << " ���͵����Լ����.").c_str());
	teleportToPlayer(target->getInGameID(), admin->getInGameID());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Get)

ADMIN_CMD_BEGIN_ATR(Jail, "�ͽ�����", 2, g_admin_player_cmd_reg)
{
	SendClientMessage(admin->getInGameID(), 0xFFFFFFFF, "[Admin] ����������δ���.");
	ADMIN_LOG(admin, target, "try to use jail function(unfinished)");
}
ADMIN_CMD_END(Jail)

ADMIN_CMD_BEGIN_ATR(Freeze, "����", 2, g_admin_player_cmd_reg)
{
	target->setFreezed(true);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ������ " << target->getLogName() << UID(target) << ". ԭ��: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Freeze)

ADMIN_CMD_BEGIN_AT(Unfreeze, "�ⶳ", 1, g_admin_player_cmd_reg)
{
	target->setFreezed(false);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" �ⶳ�� " << target->getLogName() << UID(target) << ".").c_str());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Unfreeze)

ADMIN_CMD_BEGIN_ATR(Mute, "����", 2, g_admin_player_cmd_reg)
{
	target->setMuted(true);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" �� " << target->getLogName() << UID(target) << " ����. ԭ��: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Mute)

ADMIN_CMD_BEGIN_AT(Unmute, "�����", 1, g_admin_player_cmd_reg)
{
	target->setMuted(false);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ȡ���˶� " << target->getLogName() << UID(target) << " �Ľ���.").c_str());
	ADMIN_LOG(admin, target, "");
}
ADMIN_CMD_END(Unmute)

ADMIN_CMD_BEGIN_ATR(Kill, "ɱ��", 2, g_admin_player_cmd_reg)
{
	SetPlayerHealth(target->getInGameID(), 0.0f);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ������ " << target->getLogName() << UID(target) << " ������ֵ. ԭ��: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Kill)

ADMIN_CMD_BEGIN_ATR(Warn, "����", 1, g_admin_player_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ���� " << target->getLogName() << UID(target) << ": " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(Warn)

ADMIN_CMD_BEGIN_ATR(KickPlayer, "�߳�������", 2, g_admin_player_cmd_reg)
{
	Kick(target->getInGameID());
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" �� " << target->getLogName() << UID(target) << " �߳�������. ԭ��: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(KickPlayer)

ADMIN_CMD_BEGIN_ATR(BanPlayer, "���", 3, g_admin_player_cmd_reg)
{
	target->setDisabled(true);
	GameServer::getInstance().datasource.addBanRecord(admin->_getAccountInfo(), target->getLogName(), getPlayerIP(target->getInGameID()), getGPCI(target->getInGameID()));
	Kick(target->getInGameID());
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ����� " << target->getLogName() << UID(target) << ". ԭ��: " << reason).c_str());
	ADMIN_LOG(admin, target, reason);
}
ADMIN_CMD_END(BanPlayer)

ADMIN_CMD_BEGIN_ATR(SetLevel, "���Ĺ���Ա�ȼ�", 3, g_admin_player_cmd_reg)
{
	int level = atoi(reason.c_str());
	target->setAdminLevel(admin, level);
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" �� " << target->getLogName() << UID(target) << " �Ĺ���Ա�ȼ�����Ϊ " << level << ".").c_str());
	ADMIN_LOG(admin, target, STR(level));
}
ADMIN_CMD_END(SetLevel)

// Server

ADMIN_CMD_BEGIN_AR(Announce, "����", 2, g_admin_server_cmd_reg)
{
	SendClientMessageToAll(0xFFFFFFFF, STR("[Admin] ����Ա " << admin->getLogName() << UID(admin) <<
		" ����: " << reason).c_str());
	ADMIN_LOG(admin, GameServer::getInstance().accountmanager.findAccount(-1), reason);
}
ADMIN_CMD_END(Announce)

ADMIN_CMD_BEGIN_AR(LockServer, "����������", 3, g_admin_server_cmd_reg)
{
	SendClientMessage(admin->getInGameID(), 0xFFFFFFFF, "[Admin] ����������������δ���.");
	ADMIN_LOG(admin, GameServer::getInstance().accountmanager.findAccount(-1), reason);
}
ADMIN_CMD_END(LockServer)

ADMIN_CMD_BEGIN_AR(CreateEmptyHouse, "�����հ׷���", 3, g_admin_server_cmd_reg)
{
	HouseInfo info; info.name = reason;
	GetPlayerPos(admin->getInGameID(), &info.ex, &info.ey, &info.ez);
	info.ez -= 1.0f;
	GetPlayerFacingAngle(admin->getInGameID(), &info.rotation);
	info.rotation += 90.0f;
	GameServer::getInstance().housemanager.addHouse(info);
	ADMIN_LOG(admin, GameServer::getInstance().accountmanager.findAccount(-1), "");
}
ADMIN_CMD_END(CreateEmptyHouse)

ADMIN_CMD_BEGIN_A(ReloadHouses, "���¼������з���", 3, g_admin_server_cmd_reg)
{
	GameServer::getInstance().housemanager.reloadAll();
	ADMIN_LOG(admin, GameServer::getInstance().accountmanager.findAccount(-1), "");
}
ADMIN_CMD_END(ReloadHouses)

void showAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, std::vector<AdminCommandBase*>& cmdlist)
{
	DialogItemList list;

	for (auto i : cmdlist)
	{
		if (i->requiredlevel > admin->getAdminLevel())
		{
			continue;
		}

		switch (i->type)
		{
		case ADMIN:
			list.append(i->name, std::bind(&AdminCommandBase::executeA, i, admin));
			break;
		case ADMIN_TARGET:
			list.append(i->name, std::bind(&AdminCommandBase::executeAT, i, admin, target));
			break;
		case ADMIN_REASON:
			list.append(i->name, std::bind(&showAdminInputDialog, admin,
				[admin, i](const std::string& reason) { i->executeAR(admin, reason); }));
			break;
		case ADMIN_TARGET_REASON:
			list.append(i->name, std::bind(&showAdminInputDialog, admin,
				[admin, target, i](const std::string& reason) { i->executeATR(admin, target, reason); }));
			break;
		default:
			std::cout << "[AdminCommands] Warning: Unexpected command type.\n";
		}
	}

	GameServer::getInstance().dialogmanager.displayListDialog(admin, STR("��� " << target->getLogName()), list, "ȷ��", "ȡ��");
}

void showPlayerAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target)
{
	showAdminDialog(admin, target, g_admin_player_cmd_reg);
}

void showServerAdminDialog(const std::shared_ptr<Account>& admin)
{
	showAdminDialog(admin, GameServer::getInstance().accountmanager.findAccount(-1), g_admin_server_cmd_reg);
}

void showAdminInputDialog(const std::shared_ptr<Account>& player, const std::function<void(const std::string&)>& callback)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "ԭ��", "����ԭ��", "ȷ��", "ȡ��", false, callback);
}
