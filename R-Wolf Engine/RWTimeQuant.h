//	Базовый заголовочный файл структуры трансляции времени
//	Версия 1.2.1
//	Последнее обновление: 27.03.2019
//	Не для распространения, декомпиляция и публикация запрещена
//	kalterseele, 2018 - 2019

#pragma once

struct _HPTIME {
	_HPTIME(double inTotal, double inDelta) {
		this->total = static_cast<long float>(inTotal);
		this->delta = static_cast<long float>(inDelta);
		this->buffer = 0;
		this->first = 0;
		this->second = 0;
		this->lastTick = 0;
	}
	void ADDTOBUFFER(double in) {
		this->buffer += static_cast<long float>(in);
	}
	void CLEARBUFFER() {
		this->buffer = 0;
	}
	long float CURRENTMSEC() {
		LPSYSTEMTIME st = new SYSTEMTIME;
		GetLocalTime(st);
		return static_cast<long float>(st->wMilliseconds);
		delete st;
	}
	~_HPTIME() {
		this->total = 0;
		this->delta = 0;
	}
	int operator ==(const _HPTIME* compare) {
		return (this->total == compare->total) && (this->delta == compare->delta);
	}
	long float total;
	long float delta;
	long float lastTick;
	long float buffer;
	long float first;
	long float second;
};
typedef _HPTIME RWTIMEQUANT;

