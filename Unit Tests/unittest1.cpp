#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(Parser_Tests)
	{
	public:
		
		TEST_METHOD(Test1)
		{
			Assert::AreEqual(true, Parser::isInt("1234"));
		}
		TEST_METHOD(Test2)
		{
			Assert::AreEqual(false, Parser::isInt("1234a"));
		}
	};

	TEST_CLASS(Button_Tests)
	{
	public:
		TEST_METHOD(button_coords_test)
		{
			Link l;
			l.filename = "start.xml";
			l.text = "Go to start";
			sf::Font font;
			if (font.loadFromFile("../content/font.ttf"))
			{
				button b(l, font, 100.0f, 100.0f, false);
				Assert::AreEqual(100, b.coords.top);
			}
			else Assert::Fail(L"Could not load font.ttf");
		}
	};
}