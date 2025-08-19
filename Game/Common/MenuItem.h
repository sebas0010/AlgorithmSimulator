#pragma once

#include "Core.h"
#include <string.h>
struct MenuItem
{
	// 함수 포인터 선언
	typedef void (*OnSelected)();

	MenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		size_t length = strlen(text) + 1;
		menuText = new char[length];
		strcpy_s(menuText, length, text);
	}

	~MenuItem()
	{
		SafeDeleteArray(menuText);
	}
	// 메뉴 텍스트
	char* menuText = nullptr;

	// 메뉴 선택 시 실행할 동작
	OnSelected onSelected = nullptr;
};