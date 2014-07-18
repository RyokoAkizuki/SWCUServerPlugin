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
	}, [player, house](const std::string& password) { teleportToPos(player->getInGameID(), house->_getInfo().ex, house->_getInfo().ey, house->_getInfo().ez + 1, 0, 0); });
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
		.append("传送到门口", [player, house]() {
			teleportToPos(player->getInGameID(), house->_getInfo().ex, house->_getInfo().ey, house->_getInfo().ez + 1, 0, 0);
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

