#pragma once

#include <functional>
#include <map>
#include <set>

#include "Account.h"
#include "AccountManager.h"

typedef std::function<void(const std::string&)> DialogInputCallback;
typedef std::function<void()> DialogMessageCallback;

struct DialogInfo
{
	int allocateddialogid;
	int playeringameid;
	uint64_t playersession;
	int dialogtype;
};

class DialogItemList
{
protected:
	std::map<std::string, std::pair<DialogMessageCallback, DialogMessageCallback>> listitem;

public:
	DialogItemList& append(const std::string& title,
		const DialogMessageCallback& callbackYes,
		const DialogMessageCallback& callbackNo = DialogMessageCallback()
		);
	void _callback(const std::string& itemtitle, bool yes);
	std::string serialize() const;
};

typedef std::pair<DialogInputCallback, DialogInputCallback> DialogInputBox;
typedef std::pair<DialogMessageCallback, DialogMessageCallback> DialogMessageBox;

class DialogManager
{
protected:
	std::set<int> allocatedid;
	std::map<int, DialogInfo> pending;
	std::map<int, DialogItemList> listdialog;
	std::map<int, DialogInputBox> inputdialog;
	std::map<int, DialogMessageBox> msgdialog;
	AccountManager& accountmgr;
	int lastid;

public:
	DialogManager(AccountManager& accountmgr);

	void displayInputDialog(const Account& player,
		const std::string& caption, const std::string& info,
		const std::string& btn1, const std::string& btn2, bool password,
		const DialogInputCallback& callbackYes,
		const DialogInputCallback& callbackNo = DialogInputCallback());

	void displayMessageDialog(const Account& player,
		const std::string& caption, const std::string& info,
		const std::string& btn1, const std::string& btn2,
		const DialogMessageCallback& callbackYes,
		const DialogMessageCallback& callbackNo = DialogMessageCallback());

	void displayListDialog(const Account& player,
		const std::string& caption, const DialogItemList& items,
		const std::string& btn1, const std::string& btn2);

	void _callback(int playerid, int dialogid, int response, int listitem, const char* inputtext);

protected:
	int _allocateID(int trytime = 0);
	void _dispose(int id);
};
