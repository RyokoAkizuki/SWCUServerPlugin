/*
 * Copyright 2014 Yukino Hayakawa<tennencoll@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <functional>
#include <map>
#include <set>
#include <vector>

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
	std::vector<std::string> itemname;

public:
	DialogItemList& append(const std::string& title,
		const DialogMessageCallback& callbackYes,
		const DialogMessageCallback& callbackNo = DialogMessageCallback([](){})
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

	void displayInputDialog(const std::shared_ptr<Account>& player,
		const std::string& caption, const std::string& info,
		const std::string& btn1, const std::string& btn2, bool password,
		const DialogInputCallback& callbackYes,
		const DialogInputCallback& callbackNo = DialogInputCallback([](const std::string&){}));

	void displayMessageDialog(const std::shared_ptr<Account>& player,
		const std::string& caption, const std::string& info,
		const std::string& btn1, const std::string& btn2,
		const DialogMessageCallback& callbackYes = DialogMessageCallback([](){}),
		const DialogMessageCallback& callbackNo = DialogMessageCallback([](){}));

	void displayListDialog(const std::shared_ptr<Account>& player,
		const std::string& caption, const DialogItemList& items,
		const std::string& btn1, const std::string& btn2);

	bool _callback(int playerid, int dialogid, int response, int listitem, const char* inputtext);

protected:
	int _allocateID(int trytime = 0);
	void _dispose(int id);
};
