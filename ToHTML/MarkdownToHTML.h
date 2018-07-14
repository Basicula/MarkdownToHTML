#pragma once
#include "ToHTML.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class MarkdownToHTML : public ToHTML
{
public:
	MarkdownToHTML(const std::string &fileName);

	void open() override;
	void parse() override;
	void write() override;

private:
	std::string inputFile;
	std::vector<std::string> info;
	std::vector<std::string> result;
};
