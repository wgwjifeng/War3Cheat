// InjectorWar3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <ProcessLib/Process/Process.h>
#include <CharacterLib/Character.h>
#include <InjectorLib/DllInjector/DllInjector.h>
#include <FileLib/File.h>
#include <LogLib/Log.h>
#include "ShareMemory/ConsoleShareMemory.h"

#pragma comment(lib,"ProcessLib.lib")
#pragma comment(lib,"CharacterLib.lib")
#pragma comment(lib,"FileLib.lib")
#pragma comment(lib,"LogLib.lib")
#pragma comment(lib,"InjectorLib.lib")
#define _SELF L"InjectorWar3.cpp"

BOOL InjectorDLLToGame(_In_ DWORD dwPid)
{
	std::wcout << L"Injectoring 'War3Cheat.dll' to 'War3.exe'" << std::endl;
	return libTools::CDllInjector::RemoteThreadInjectorDLL(dwPid, libTools::CCharacter::MakeCurrentPath(L"\\War3Cheat.dll"));
}


VOID PrintHelp()
{
	std::wcout << L"------------------------------------------------------" << std::endl;
	std::wcout << L"SetSelectedObjectInvincible" << std::endl;
	std::wcout << L"SetSelectedObjectAttackType" << std::endl;
	std::wcout << L"SetSelectedHeroSkillCool" << std::endl;
	std::wcout << L"PrintItem" << std::endl;
	std::wcout << L"ChangeItem -Index -ItemId" << std::endl;
	std::wcout << L"------------------------------------------------------" << std::endl;
}

BOOL CreateMemoryShare()
{
	if (!CConsoleShareMemory::GetInstance().Create(::GetStdHandle(STD_OUTPUT_HANDLE)))
	{
		std::wcout << CConsoleShareMemory::GetInstance().GetErrorText().c_str() << std::endl;
		return FALSE;
	}

	libTools::CLog::GetInstance().SetClientName(L"InjectorWar3", libTools::CCharacter::MakeCurrentPath(L"\\"));
	libTools::CLog::GetInstance().SetSyncSendLog();
	return TRUE;
}

BOOL ConnectGame(_In_ DWORD dwMaxTimeout)
{	
	for (ULONGLONG ulTick = ::GetTickCount64(); ::GetTickCount64() - ulTick <= static_cast<ULONGLONG>(dwMaxTimeout);)
	{
		std::wcout << L"Connecting Game ... " << std::endl;
		if (CConsoleShareMemory::GetInstance().IsConnectedGame())
		{
			return TRUE;
		}


		::Sleep(1000);
	}
	return FALSE;
}

int main()
{
	if (!libTools::CFile::FileExist(libTools::CCharacter::MakeCurrentPath(L"\\War3Cheat.dll")))
	{
		std::wcout << L"UnExist 'War3Cheat.dll'" << std::endl;
		system("pause");
		return 0;
	}
	else if (!CreateMemoryShare())
	{
		system("pause");
		return 0;
	}

	libTools::CCharacter::SetConsoleLanguage();
	libTools::CCharacter::SetSpecialCharacterMode();

	DWORD dwPid = libTools::CProcess::FindPidByProcName(L"War3.exe");
	if (dwPid == 0xFFFFFFFF)
	{
		std::wcout << L"UnExist Process 'War3.exe'" << std::endl;
		system("pause");
		return 0;
	}

	
	if (!InjectorDLLToGame(dwPid))
	{
		std::wcout << L"Injector DLL to Process[" << dwPid << "] Faild! please run as administrator" << std::endl;
		system("pause");
		return 0;
	}


	if (!ConnectGame(10 * 1000))
	{
		std::wcout << L"Connect Game Timeout ..." << std::endl;
		system("pause");
		return 0;
	}


	PrintHelp();
	for (;;)
	{
		std::wcout << L">";


		std::wstring wsCommand;
		std::getline(std::wcin, wsCommand);
		wsCommand = libTools::CCharacter::MakeTextToLower(wsCommand);

		if (wsCommand == L"help")
		{
			PrintHelp();
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"SetSelectedObjectInvincible")))
		{
			CConsoleShareMemory::GetInstance().SetNormalAction(em_Action_Type::SetSelectedObjectInvincible);
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"SetSelectedObjectAttackType")))
		{
			CConsoleShareMemory::GetInstance().SetNormalAction(em_Action_Type::SetSelectedObjectAttackType);
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"SetSelectedHeroSkillCool")))
		{
			CConsoleShareMemory::GetInstance().SetNormalAction(em_Action_Type::SetSelectedHeroSkillCool);
		}
		else if (wsCommand == libTools::CCharacter::MakeTextToLower(std::wstring(L"PrintItem")))
		{
			CConsoleShareMemory::GetInstance().SetNormalAction(em_Action_Type::PrintItem);
		}
		else if (wsCommand.find(libTools::CCharacter::MakeTextToLower(std::wstring(L"ChangeItem"))) != -1)
		{
			libTools::CCharacter::GetRemoveRight(wsCommand, L"ChangeItem", wsCommand);
			CConsoleShareMemory::GetInstance().SetActionWithParam(em_Action_Type::ChangeItem, wsCommand);
		}
		else
		{
			std::wcout << L"Invalid Command" << std::endl;
			PrintHelp();
		}
	}
}

