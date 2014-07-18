#include <sampgdk/a_samp.h>
#include <sampgdk/a_players.h>

#include "HouseDialogs.h"
#include "Teleport.h"
#include "GameServer.h"

void showHouseAuthDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "进入限制区域", "请输入密码", "进入", "离开", false,
	[player, house](const std::string& password) {
		if (house->auth(password))
		{
			SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[House] 验证成功");
		}
		else
		{
			SendClientMessage(player->getInGameID(), 0xFFFFFFFF, "[House] 密码错误");
			showHouseAuthDialog(player, house);
		}
	}, [player, house](const std::string& password) { teleportToPos(player->getInGameID(), house->_getInfo().tx, house->_getInfo().ty, house->_getInfo().tz + 1, 0, 0); });
}

void showHouseListDialog(const std::shared_ptr<Account>& player)
{
	std::vector<HouseInfo> dest;
	DialogItemList list;
	GameServer::getInstance().datasource.loadHouses(dest, player->getUserID());
	for (auto& i : dest)
	{
		list.append(i.name, [i, player](){
			showHouseManageDialog(player, GameServer::getInstance().housemanager.findHouse(i.houseid));
		});
	}
	GameServer::getInstance().dialogmanager.displayListDialog(player, "我的房屋", list, "查看", "取消");
}

void showHouseManageDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	DialogItemList list;
	list
		.append("更改名称", std::bind(&showHouseChangeNameDialog, player, house))
		.append("将门牌放在我站的位置", [player, house]() {
			float pos[3], rotation;
			GetPlayerPos(player->getInGameID(), &pos[0], &pos[1], &pos[2]);
			pos[2] -= 1.0f;
			GetPlayerFacingAngle(player->getInGameID(), &rotation);
			rotation += 90.0f;
			house->setEntrance(pos[0], pos[1], pos[2], rotation);
		})
		.append("更改密码", std::bind(&showHouseChangePasswordDialog, player, house))
		.append("将门口设置在我站的位置", [player, house]() {
			float pos[3];
			GetPlayerPos(player->getInGameID(), &pos[0], &pos[1], &pos[2]);
			house->setTeleportPos(pos[0], pos[1], pos[2]);
		})
		.append("传送到门口", [player, house]() {
			teleportToPos(player->getInGameID(), house->_getInfo().tx, house->_getInfo().ty, house->_getInfo().tz + 1, 0, 0);
		})
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, house->getName(), list, "确定", "取消");
}

void showHouseChangeNameDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "更改房屋名称", "请输入新的名称",
		"确定", "取消", false,
		[house](const std::string& newname) { house->setName(newname); }
	);
}

void showHouseChangePasswordDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "更改房屋密码", "请输入新的密码",
		"确定", "取消", false,
		[house](const std::string& newpw) { house->setPassword(newpw); }
	);
}
