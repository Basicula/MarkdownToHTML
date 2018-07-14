#include "MarkdownToHTML.h"

enum Tags
{
	TEXT,
	MAINHEADER,
	SUBHEADER,
	PARAGRAPH,
	OLIST,
	ULIST,
};

MarkdownToHTML::MarkdownToHTML(const std::string &fileName)
	:inputFile(fileName)
{}

void MarkdownToHTML::open()
{
	std::ifstream input(inputFile);
	std::string line;
	while (std::getline(input, line))
	{
		info.push_back(line);
	}
}

bool isSubHeader(const std::string &line)
{
	if (line[0] == line[1] && line[1] == '#')
		return true;
	else
		return false;

}

bool isMainHeader(const std::string &line)
{
	for (int i = 1; i < line.size(); ++i)
		if (line[i] != line[i - 1] || line[i] != '=')
			return false;
	return true;
}

int boldCount(const std::string &line)
{
	int bold = 0;
	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i] == '*' && i + 1 < line.size() && line[i + 1] == '*')
			bold++;
	}
	return bold;
}

int italicCount(const std::string &line)
{
	int italic = 0;
	for (int i = 0; i < line.size(); ++i)
	{
		if (line[i] == '_')
			italic++;
	}
	return italic;
}


bool isOList(const std::string &line)
{
	int i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] >= '1' &&line[i + 1] == '.')
		return true;
	else
		return false;
}

bool isUList(const std::string &line)
{
	int i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '*' &&line[i + 1] == ' ')
		return true;
	else
		return false;
}

Tags analyzeLine(const std::string &line)
{
	if (line == "") return PARAGRAPH;
	if (isMainHeader(line)) return MAINHEADER;
	if (isSubHeader(line)) return SUBHEADER;
	if (isOList(line)) return OLIST;
	if (isUList(line)) return ULIST;
	return TEXT;
}

void makeBold(std::string &line)
{
	int bold = boldCount(line);
	if (bold < 2) return;
	line += ' ';
	std::string start = "<strong>";
	std::string finish = "</strong>";
	if (bold % 2 == 0)
	{
		int p = 0;
		for (int i = 0; i < line.size() - 1; ++i)
		{
			if (line[i] == line[i + 1] && line[i] == '*')
			{
				line = line.substr(0, i) + ((p == 0) ? start : finish) + line.substr(i + 2);
				p = (p + 1) % 2;
			}
		}
	}
	else
	{
		int i = 0, j = line.size() - 2;
		while (1)
		{
			if (line[i] != line[i + 1] || line[i] != '*')
				++i;
			else if (line[j] != line[j + 1] || line[j] != '*')
				--j;
			else
				break;
		}
		line = line.substr(0, i) + start + line.substr(i + 2, j - i - 2) + finish + line.substr(j + 2);
	}
}

void makeItalic(std::string &line)
{
	int italic = italicCount(line);
	if (italic < 2) return;
	line += ' ';
	std::string start = "<em>";
	std::string finish = "</em>";
	if (italic % 2 == 0)
	{
		int p = 0;
		for (int i = 0; i < line.size() - 1; ++i)
		{
			if (line[i] == '_')
			{
				line = line.substr(0, i) + ((p == 0) ? start : finish) + line.substr(i + 1);
				p = (p + 1) % 2;
			}
		}
	}
	else
	{
		int i = 0, j = line.size() - 2;
		while (1)
		{
			if (line[i] != '_')
				++i;
			else if (line[j] != '_')
				--j;
			else
				break;
		}
		line = line.substr(0, i) + start + line.substr(i + 2, j - i - 2) + finish + line.substr(j + 2);
	}
}

std::string makeGoodText(const std::string &line)
{
	std::string result = line;
	makeBold(result);
	makeItalic(result);
	return result;
}

std::string makeParagraph(const std::string &line)
{
	return ("<p>" + makeGoodText(line) + "</p>\n");
}

std::string makeMainHeader(const std::string &line)
{
	return ("<h1>" + makeGoodText(line) + "</h1>\n");
}

std::string makeSubHeader(const std::string &line)
{
	int i = 0;
	while (line[i++] != '#');
	return ("<h2>" + makeGoodText(line.substr(i)) + "</h2>\n");
}

std::string listElementContent(const std::string &line)
{
	int i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '*') return line.substr(i + 2);
	else return line.substr(i + 3);
}

void MarkdownToHTML::parse()
{
	for (int i = 0; i < info.size();)
	{
		Tags tag = analyzeLine(info[i]);
		switch (tag)
		{
		case TEXT:
			++i;
			break;
		case MAINHEADER:
			result.push_back(makeMainHeader(info[i - 1]));
			i += 2;
			break;
		case SUBHEADER:
			result.push_back(makeSubHeader(info[i]));
			i += 2;
			break;
		case PARAGRAPH:
			result.push_back(makeParagraph(info[i - 1]));
			++i;
			break;
		case OLIST:
			result.push_back("<ol>");
			while (isOList(info[i]))
			{
				result.push_back("<li>" + makeGoodText(listElementContent(info[i])) + "</li>");
				++i;
			}
			result.push_back("</ol>\n");
			++i;
			break;
		case ULIST:
			result.push_back("<ul>");
			while (isUList(info[i]))
			{
				result.push_back("<li>" + makeGoodText(listElementContent(info[i])) + "</li>");
				++i;
			}
			result.push_back("</ul>\n");
			++i;
			break;
		default:
			break;
		}
	}
}

void MarkdownToHTML::write()
{
	std::ofstream out("test.html");
	for (int i = 0; i < result.size(); ++i)
		out << result[i] << std::endl;
}