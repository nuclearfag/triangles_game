//	Базовый заголовочный файл структуры HardwareSync
//	Версия 1.2.1
//	Последнее обновление: 16.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <wchar.h>
#include <iostream>
#include <string>
#include <sstream>
#include "RWVectorMath.h"
using namespace RWBinaryOp;

#define method inline auto

namespace RWHardwareSync {
	__declspec(selectany) unsigned char* signalBuffer = new unsigned char[32]{ 0 };
	enum Keys {
		kShift = 0x10,
		k1 = 0x30,
		k2,
		k3,
		k4,
		k5,
		k6,
		k7,
		k8,
		k9,
		k0,
		kA = 0x41,
		kB,
		kC,
		kD,
		kE,
		kF,
		kG,
		kH,
		kI,
		kJ,
		kK,
		kL,
		kM,
		kN,
		kO,
		kP,
		kQ,
		kR,
		kS,
		kT,
		kU,
		kV,
		kW,
		kX,
		kY,
		kZ,
		kConsole = 0xC0
	};
	method captureKey(int requestedKey) 
	{
		if (GetForegroundWindow() == getRenderTarget()->GetHwnd() && 
			GetAsyncKeyState(requestedKey)) return true;
		else return false;
	}
	method catchSignal(unsigned char* throwedKey, RWLevel* level)
	{
		for (int i = 0; i < 193; i++)
		{
			if (cellOperation(throwedKey[i / 8], READ, i % 8))
			{
				switch (i)
				{
				case kL:
					level->drawLagometer();
					break;
				case kConsole:
					level->openConsole();
					break;
				default:
					break;
				}
			}
		}
	}
	method throwSignal(int throwedKey, unsigned char* &signal) 
	{
		if (cellOperation(signal[throwedKey / 8], READ, throwedKey % 8))
		{
			cellOperation(signal[throwedKey / 8], WRITE, throwedKey % 8);
		}
		else
		{
			cellOperation(signal[throwedKey / 8], WRITE, throwedKey % 8);
		}
	}
	method convertKeyToPTCC(int requestedKey) {
		std::string output;
		switch (requestedKey) 
		{
		// 1 - 0
		case 0x30:
			if (captureKey(VK_SHIFT)) output = "!";
			else output = "1";
			return output.c_str();
			break;
		case 0x31:
			if (captureKey(VK_SHIFT)) output = "\"";
			else output = "2";
			return output.c_str();
			break;
		case 0x32:
			if (captureKey(VK_SHIFT)) output = "#";
			else output = "3";
			return output.c_str();
			break;
		case 0x33:
			if (captureKey(VK_SHIFT)) output = "$";
			else output = "4";
			return output.c_str();
			break;
		case 0x34:
			if (captureKey(VK_SHIFT)) output = "%";
			else output = "5";
			return output.c_str();
			break;
		case 0x35:
			if (captureKey(VK_SHIFT)) output = "^";
			else output = "6";
			return output.c_str();
			break;
		case 0x36:
			if (captureKey(VK_SHIFT)) output = "&";
			else output = "7";
			return output.c_str();
			break;
		case 0x37:
			if (captureKey(VK_SHIFT)) output = "*";
			else output = "8";
			return output.c_str();
			break;
		case 0x38:
			if (captureKey(VK_SHIFT)) output = "(";
			else output = "9";
			return output.c_str();
			break;
		case 0x39:
			if (captureKey(VK_SHIFT)) output = ")";
			else output = "0";
			return output.c_str();
			break;
		// A - Z
		case 0x41:
			if (captureKey(VK_SHIFT)) output = "A";
			else output = "a";
			return output.c_str();
			break;
		case 0x42:
			if (captureKey(VK_SHIFT)) output = "B";
			else output = "b";
			return output.c_str();
			break;
		case 0x43:
			if (captureKey(VK_SHIFT)) output = "C";
			else output = "c";
			return output.c_str();
			break;
		case 0x44:
			if (captureKey(VK_SHIFT)) output = "D";
			else output = "d";
			return output.c_str();
			break;
		case 0x45:
			if (captureKey(VK_SHIFT)) output = "E";
			else output = "e";
			return output.c_str();
			break;
		case 0x46:
			if (captureKey(VK_SHIFT)) output = "F";
			else output = "f";
			return output.c_str();
			break;
		case 0x47:
			if (captureKey(VK_SHIFT)) output = "G";
			else output = "g";
			return output.c_str();
			break;
		case 0x48:
			if (captureKey(VK_SHIFT)) output = "H";
			else output = "h";
			return output.c_str();
			break;
		case 0x49:
			if (captureKey(VK_SHIFT)) output = "I";
			else output = "i";
			return output.c_str();
			break;
		case 0x4A:
			if (captureKey(VK_SHIFT)) output = "J";
			else output = "j";
			return output.c_str();
			break;
		case 0x4B:
			if (captureKey(VK_SHIFT)) output = "K";
			else output = "k";
			return output.c_str();
			break;
		case 0x4C:
			if (captureKey(VK_SHIFT)) output = "L";
			else output = "l";
			return output.c_str();
			break;
		case 0x4D:
			if (captureKey(VK_SHIFT)) output = "M";
			else output = "m";
			return output.c_str();
			break;
		case 0x4E:
			if (captureKey(VK_SHIFT)) output = "N";
			else output = "n";
			return output.c_str();
			break;
		case 0x4F:
			if (captureKey(VK_SHIFT)) output = "O";
			else output = "o";
			return output.c_str();
			break;
		case 0x50:
			if (captureKey(VK_SHIFT)) output = "P";
			else output = "p";
			return output.c_str();
			break;
		case 0x51:
			if (captureKey(VK_SHIFT)) output = "Q";
			else output = "q";
			return output.c_str();
			break;
		case 0x52:
			if (captureKey(VK_SHIFT)) output = "R";
			else output = "r";
			return output.c_str();
			break;
		case 0x53:
			if (captureKey(VK_SHIFT)) output = "S";
			else output = "s";
			return output.c_str();
			break;
		case 0x54:
			if (captureKey(VK_SHIFT)) output = "T";
			else output = "t";
			return output.c_str();
			break;
		case 0x55:
			if (captureKey(VK_SHIFT)) output = "U";
			else output = "u";
			return output.c_str();
			break;
		case 0x56:
			if (captureKey(VK_SHIFT)) output = "V";
			else output = "v";
			return output.c_str();
			break;
		case 0x57:
			if (captureKey(VK_SHIFT)) output = "W";
			else output = "w";
			return output.c_str();
			break;
		case 0x58:
			if (captureKey(VK_SHIFT)) output = "X";
			else output = "x";
			return output.c_str();
			break;
		case 0x59:
			if (captureKey(VK_SHIFT)) output = "Y";
			else output = "y";
			return output.c_str();
			break;
		case 0x5A:
			if (captureKey(VK_SHIFT)) output = "Z";
			else output = "z";
			return output.c_str();
			break;
		// Специальные символы

		default:
			break;
		}
	}
	method convertKeyToPTCWC(int requestedKey) {
		std::wstring output;
		switch (requestedKey) 
		{
		// 1 - 0
		case 0x30:
			if (captureKey(VK_SHIFT)) output = L"!";
			else output = L"1";
			return output.c_str();
			break;
		case 0x31:
			if (captureKey(VK_SHIFT)) output = L"\"";
			else output = L"2";
			return output.c_str();
			break;
		case 0x32:
			if (captureKey(VK_SHIFT)) output = L"#";
			else output = L"3";
			return output.c_str();
			break;
		case 0x33:
			if (captureKey(VK_SHIFT)) output = L"$";
			else output = L"4";
			return output.c_str();
			break;
		case 0x34:
			if (captureKey(VK_SHIFT)) output = L"%";
			else output = L"5";
			return output.c_str();
			break;
		case 0x35:
			if (captureKey(VK_SHIFT)) output = L"^";
			else output = L"6";
			return output.c_str();
			break;
		case 0x36:
			if (captureKey(VK_SHIFT)) output = L"&";
			else output = L"7";
			return output.c_str();
			break;
		case 0x37:
			if (captureKey(VK_SHIFT)) output = L"*";
			else output = L"8";
			return output.c_str();
			break;
		case 0x38:
			if (captureKey(VK_SHIFT)) output = L"(";
			else output = L"9";
			return output.c_str();
			break;
		case 0x39:
			if (captureKey(VK_SHIFT)) output = L")";
			else output = L"0";
			return output.c_str();
			break;
			// A - Z
		case 0x41:
			if (captureKey(VK_SHIFT)) output = L"A";
			else output = L"a";
			return output.c_str();
			break;
		case 0x42:
			if (captureKey(VK_SHIFT)) output = L"B";
			else output = L"b";
			return output.c_str();
			break;
		case 0x43:
			if (captureKey(VK_SHIFT)) output = L"C";
			else output = L"c";
			return output.c_str();
			break;
		case 0x44:
			if (captureKey(VK_SHIFT)) output = L"D";
			else output = L"d";
			return output.c_str();
			break;
		case 0x45:
			if (captureKey(VK_SHIFT)) output = L"E";
			else output = L"e";
			return output.c_str();
			break;
		case 0x46:
			if (captureKey(VK_SHIFT)) output = L"F";
			else output = L"f";
			return output.c_str();
			break;
		case 0x47:
			if (captureKey(VK_SHIFT)) output = L"G";
			else output = L"g";
			return output.c_str();
			break;
		case 0x48:
			if (captureKey(VK_SHIFT)) output = L"H";
			else output = L"h";
			return output.c_str();
			break;
		case 0x49:
			if (captureKey(VK_SHIFT)) output = L"I";
			else output = L"i";
			return output.c_str();
			break;
		case 0x4A:
			if (captureKey(VK_SHIFT)) output = L"J";
			else output = L"j";
			return output.c_str();
			break;
		case 0x4B:
			if (captureKey(VK_SHIFT)) output = L"K";
			else output = L"k";
			return output.c_str();
			break;
		case 0x4C:
			if (captureKey(VK_SHIFT)) output = L"L";
			else output = L"l";
			return output.c_str();
			break;
		case 0x4D:
			if (captureKey(VK_SHIFT)) output = L"M";
			else output = L"m";
			return output.c_str();
			break;
		case 0x4E:
			if (captureKey(VK_SHIFT)) output = L"N";
			else output = L"n";
			return output.c_str();
			break;
		case 0x4F:
			if (captureKey(VK_SHIFT)) output = L"O";
			else output = L"o";
			return output.c_str();
			break;
		case 0x50:
			if (captureKey(VK_SHIFT)) output = L"P";
			else output = L"p";
			return output.c_str();
			break;
		case 0x51:
			if (captureKey(VK_SHIFT)) output = L"Q";
			else output = L"q";
			return output.c_str();
			break;
		case 0x52:
			if (captureKey(VK_SHIFT)) output = L"R";
			else output = L"r";
			return output.c_str();
			break;
		case 0x53:
			if (captureKey(VK_SHIFT)) output = L"S";
			else output = L"s";
			return output.c_str();
			break;
		case 0x54:
			if (captureKey(VK_SHIFT)) output = L"T";
			else output = L"t";
			return output.c_str();
			break;
		case 0x55:
			if (captureKey(VK_SHIFT)) output = L"U";
			else output = L"u";
			return output.c_str();
			break;
		case 0x56:
			if (captureKey(VK_SHIFT)) output = L"V";
			else output = L"v";
			return output.c_str();
			break;
		case 0x57:
			if (captureKey(VK_SHIFT)) output = L"W";
			else output = L"w";
			return output.c_str();
			break;
		case 0x58:
			if (captureKey(VK_SHIFT)) output = L"X";
			else output = L"x";
			return output.c_str();
			break;
		case 0x59:
			if (captureKey(VK_SHIFT)) output = L"Y";
			else output = L"y";
			return output.c_str();
			break;
		case 0x5A:
			if (captureKey(VK_SHIFT)) output = L"Z";
			else output = L"z";
			return output.c_str();
			break;
			// Специальные символы

		default:
			break;
		}
	}
	method getNativeScreenSize(unsigned int& width, unsigned int& height) {
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	method getInputString(bool isWide, bool& threadAlive) {
		std::string output;
		std::wstring _output;
		if (isWide)
		{

		}
		else
		{

		}
	}
}