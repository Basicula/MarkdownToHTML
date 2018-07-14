#pragma once

class ToHTML
{
public:
	virtual void open() = 0;
	virtual void parse() = 0;
	virtual void write() = 0;
	virtual ~ToHTML() {};
};
