#include <sampgdk/a_samp.h>
#include <sstream>
#include "DialogManager.h"

const int g_maxDialogID = 32767;

DialogItemList& DialogItemList::append(const std::string& title,
	const DialogMessageCallback& callbackYes,
	const DialogMessageCallback& callbackNo)
{
	listitem.insert(std::make_pair(title, std::make_pair(callbackYes, callbackNo)));
	itemname.push_back(title);
	return *this;
}

void DialogItemList::_callback(const std::string& itemtitle, bool yes)
{
	if (itemname.empty())
	{
		return;
	}
	auto iter = listitem.find(itemtitle);
	if (iter == listitem.end())
	{
		return;
	}
	yes ? iter->second.first() : iter->second.second();
}

std::string DialogItemList::serialize() const
{
	std::stringstream stream;
	if (itemname.empty())
	{
		stream << "<无可用项目>\n";
	}
	else
	{
		for (auto& i : itemname)
		{
			stream << i << "\n";
		}
	}
	return stream.str();
}

DialogManager::DialogManager(AccountManager& accountmanager) : lastid(0), accountmgr(accountmanager) {}

void DialogManager::displayInputDialog(const std::shared_ptr<Account>& player,
	const std::string& caption, const std::string& info,
	const std::string& btn1, const std::string& btn2, bool password,
	const DialogInputCallback& callbackYes,
	const DialogInputCallback& callbackNo)
{
	int id = _allocateID();
	int style = password ? DIALOG_STYLE_PASSWORD : DIALOG_STYLE_INPUT;
	ShowPlayerDialog(player->getInGameID(), id, style,
		caption.c_str(), info.c_str(), btn1.c_str(), btn2.c_str());
	DialogInfo dlginfo = { id, player->getInGameID(), player->getSessionID(), style };
	pending.insert(std::make_pair(id, dlginfo));
	inputdialog.insert(std::make_pair(id, std::make_pair(callbackYes, callbackNo)));
}

void DialogManager::displayMessageDialog(const std::shared_ptr<Account>& player,
	const std::string& caption, const std::string& info,
	const std::string& btn1, const std::string& btn2,
	const DialogMessageCallback& callbackYes,
	const DialogMessageCallback& callbackNo)
{
	int id = _allocateID();
	ShowPlayerDialog(player->getInGameID(), id, DIALOG_STYLE_MSGBOX,
		caption.c_str(), info.c_str(), btn1.c_str(), btn2.c_str());
	DialogInfo dlginfo = { id, player->getInGameID(), player->getSessionID(), DIALOG_STYLE_MSGBOX };
	pending.insert(std::make_pair(id, dlginfo));
	msgdialog.insert(std::make_pair(id, std::make_pair(callbackYes, callbackNo)));
}

void DialogManager::displayListDialog(const std::shared_ptr<Account>& player,
	const std::string& caption, const DialogItemList& items,
	const std::string& btn1, const std::string& btn2)
{
	int id = _allocateID();
	ShowPlayerDialog(player->getInGameID(), id, DIALOG_STYLE_LIST,
		caption.c_str(), items.serialize().c_str(), btn1.c_str(), btn2.c_str());
	DialogInfo dlginfo = { id, player->getInGameID(), player->getSessionID(), DIALOG_STYLE_LIST };
	pending.insert(std::make_pair(id, dlginfo));
	listdialog.insert(std::make_pair(id, items));
}

bool DialogManager::_callback(int playerid, int dialogid, int response, int listitem, const char* inputtext)
{
	auto iter = pending.find(dialogid);
	if (iter == pending.end())
	{
		std::cout << "[DialogManager] Warning: Nonexistent dialog id.\n";
		return false;
	}
	if (accountmgr.findAccount(playerid)->getSessionID() != iter->second.playersession)
	{
		std::cout << "[DialogManager] Warning: Player session id dismatches. Callback ignored.\n";
		_dispose(dialogid);
		return false;
	}

	switch (iter->second.dialogtype)
	{
	case DIALOG_STYLE_INPUT:
	case DIALOG_STYLE_PASSWORD:
	{
		auto input = inputdialog.find(dialogid);
		if (input == inputdialog.end())
		{
			std::cout << "[DialogManager] Warning: Dialog type dismatches.\n";
			break;
		}
		if (response == 1)
		{
			input->second.first(inputtext);
		}
		else
		{
			input->second.second(inputtext);
		}
		break;
	}
	case DIALOG_STYLE_MSGBOX:
	{
		auto msg = msgdialog.find(dialogid);
		if (msg == msgdialog.end())
		{
			std::cout << "[DialogManager] Warning: Dialog type dismatches.\n";
			break;
		}
		if (response == 1)
		{
			msg->second.first();
		}
		else
		{
			msg->second.second();
		}
		break;
	}
	case DIALOG_STYLE_LIST:
	{
		auto list = listdialog.find(dialogid);
		if (list == listdialog.end())
		{
			std::cout << "[DialogManager] Warning: Dialog type dismatches.\n";
			break;
		}
		list->second._callback(inputtext, response == 1);
		break;
	}
	default:
		std::cout << "[DialogManager] Warning: Invalid dialog type.\n";
	}

	_dispose(dialogid);
	return true;
}

int DialogManager::_allocateID(int trytime)
{
	if (trytime > 10)
	{
		std::cout << "[DialogManager] Warning: Dialog ID conflict remain unsolved after 10 trys. Give up.\n";
		return -1;
	}

	int id = (++lastid) % g_maxDialogID;

	if (allocatedid.count(id))
	{
		std::cout << "[DialogManager] Warning: Dialog ID allocation conflicts.\n";

		auto iter = pending.find(id);

		if (iter == pending.end()) // shouldn't be
		{
			std::cout << "[DialogManager] Warning: Unexpected key not found.\n";
			return -1;
		}

		// previous player session changed
		if (accountmgr.findAccount(iter->second.playeringameid)->getSessionID() != iter->second.playersession)
		{
			_dispose(id);
			std::cout << "[DialogManager] Conflict resolved. Previous player is offline.\n";
		}
		else
		{
			std::cout << "[DialogManager] Conflict remains. Try another id.\n";
			return _allocateID(trytime + 1);
		}
	}

	allocatedid.insert(id); // record dialog id allocation
	return id;
}

void DialogManager::_dispose(int id)
{
	allocatedid.erase(id);
	pending.erase(id);
	listdialog.erase(id);
	inputdialog.erase(id);
	msgdialog.erase(id);
}

