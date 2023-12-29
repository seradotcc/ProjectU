#pragma once

#include <atomic>
#include <Windows.h>

class Module
{
public:
	bool isOff = true;
public:
	Module() {};
	virtual ~Module() {};
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void TurnOff() { isOff = true; }
	void TurnOn() { isOff = false; }
};