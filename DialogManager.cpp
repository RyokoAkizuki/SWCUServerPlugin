#include <sampgdk/a_samp.h>

#include "DialogManager.h"

DialogManager::DialogManager() : lastid(0) {}

void DialogManager::displayInputDialog(int playerid,
	const std::string& caption, const std::string& info,
	const std::string& btn1, const std::string& btn2, bool password,
	const DialogCallback& callbackYes, const DialogCallback& callbackNo
	)
{
	int id = _getId();
	ShowPlayerDialog(playerid, id, password ? DIALOG_STYLE_PASSWORD : DIALOG_STYLE_INPUT,
		caption.c_str(), info.c_str(), btn1.c_str(), btn2.c_str());
	pending.insert(std::make_pair(id, std::make_pair(callbackYes, callbackNo)));
}

void DialogManager::_callback(int playerid, int dialogid, int response, int listitem, const char* inputtext)
{
	auto iter = pending.find(dialogid);
	if (iter == pending.end())
	{
		return;
	}
	if (response == 1)
	{
		iter->second.first(inputtext);
	}
	else
	{
		iter->second.second(inputtext);
	}
	pending.erase(iter);
}

int DialogManager::_getId()
{
	return (++lastid) % 32767;
}

void DialogManager::_dispose(int id)
{
	pending.erase(id);
}
