#include <iostream>
#include "MarkdownToHTML.h"

int main()
{
	MarkdownToHTML test("test.txt");
	test.open();
	test.parse();
	test.write();
	system("pause");
	return 0;
}