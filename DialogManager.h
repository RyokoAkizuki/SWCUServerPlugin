#pragma once

#include <functional>
#include <map>

typedef std::function<void(const std::string&)> DialogCallback;

class DialogManager
{
protected:
	// <id, <callbackYes, callbackNo>>
	std::map<int, std::pair<DialogCallback, DialogCallback>> pending;
	int lastid;

public:
	DialogManager();

	void displayInputDialog(int playerid,
		const std::string& caption, const std::string& info,
		const std::string& btn1, const std::string& btn2, bool password,
		const DialogCallback& callbackYes,
		const DialogCallback& callbackNo = DialogCallback()
		);

	void _callback(int playerid, int dialogid, int response, int listitem, const char* inputtext);

protected:
	int _getId();
	void _dispose(int id);
};
