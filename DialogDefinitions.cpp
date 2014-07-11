#include <sampgdk/a_players.h>
#include <sstream>

#include "DialogDefinitions.h"
#include "GameServer.h"

void showLoginDialog(Account& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "��¼", "����������",
		"��¼", "�˳�������", false,
		[&player](const std::string& pw) {
			if (!player.auth(pw))
			{
				showLoginDialog(player);
			}
		},
		[&player](const std::string& pw) { Kick(player.getInGameID()); }
	);
}

void showRegisterDialog(Account& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "ע��", "����������",
		"ע��", "�˳�������", false,
		[&player](const std::string& pw) { player.create(pw); },
		[&player](const std::string& pw) { Kick(player.getInGameID()); }
	);
}

void showChangePasswordDialog(Account& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "��������", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[&player](const std::string& pw) { player.changePassword(pw); }
	);
}

void showSetLogNameDialog(Account& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "���ĵ�¼��", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[&player](const std::string& newname) { player.setLogName(newname); }
	);
}

void showSetNicknameDialog(Account& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "������ʾ��", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[&player](const std::string& name) { player.setNickname(name); }
	);
}

void showViewAccountDetailDialog(Account& player)
{
	std::stringstream str;
	str << "ID: " << player.getUserID() <<
		"\n��¼��: " << player.getLogName() <<
		"\n��ʾ��: " << player.getNickname() <<
		"\n���п����: " << player.getMoney() <<
		"\n����Ա�ȼ�: " << player.getAdminLevel();
	GameServer::getInstance().dialogmanager.displayMessageDialog(player,
		"�ҵ���Ϣ", str.str(), "ȷ��", "");
}

void showAccountOptionsDialog(Account& player)
{
	DialogItemList list;
	list
		.append("�鿴�ҵ���Ϣ", std::bind(&showViewAccountDetailDialog, std::ref(player)))
		.append("��������", std::bind(&showChangePasswordDialog, std::ref(player)))
		.append("���ĵ�¼��", std::bind(&showSetLogNameDialog, std::ref(player)))
		.append("������ʾ��", std::bind(&showSetNicknameDialog, std::ref(player)))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, "�ҵ��˺�", list, "ȷ��", "ȡ��");
}
