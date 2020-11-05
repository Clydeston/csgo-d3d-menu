#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "drawing.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define max_menu_items 15

namespace Colours
{
	D3DCOLOR Black = D3DCOLOR_ARGB(150, 000, 000, 000);
	D3DCOLOR Green = D3DCOLOR_ARGB(255, 000, 255, 000);
	D3DCOLOR Grey = D3DCOLOR_ARGB(255, 128, 128, 128);
	D3DCOLOR Yellow = D3DCOLOR_ARGB(255, 255, 255, 000);
	D3DCOLOR White = D3DCOLOR_ARGB(255, 255, 255, 255);
	D3DCOLOR Red = D3DCOLOR_ARGB(255, 255, 000, 000);
}

namespace D3DMenu
{
	class Item
	{
	public:
		const char* name;
		bool* enabled;
		bool defaultToggled;
		bool isTitle;
		int title;
	};

	// menu item objects
	Item menuItems[max_menu_items];

	namespace Settings
	{
		namespace Menu
		{
			Vec2 Position;
			int Height = 400;
			int Width = 200;
			int OutlineThickness = 1;		

			Vec2 TopRightCorner;
			Vec2 TopLeftCorner;
			Vec2 BottomRightCorner;
			Vec2 BottomLeftCorner;
		}

		namespace Colour
		{			
			D3DCOLOR Outline = Colours::Red;
			D3DCOLOR Item = Colours::Grey;
			D3DCOLOR Title = Colours::Yellow;
			D3DCOLOR ActiveItem = Colours::White;
			D3DCOLOR ActiveTitle = Colours::Red;
		}
		
		namespace Items
		{
			int Padding = 16;
			int TitleXPos = 0;
			int ItemXPos = 0;
			int TotalItemCount = 0;
			int CurrentMenuItem = 0;
			int StartingYPos = 41;
		}
	}

	namespace Header
	{
		// menu settings simplified namespace
		namespace M = D3DMenu::Settings::Menu;		
		Vec2 TextPos;
		Vec2 RightPoint;
		Vec2 LeftPoint;
		void Draw()
		{
			TextPos.x = (M::Position.x + (M::Position.x + M::Width)) / 2;
			TextPos.y = M::Position.y + 8;

			RightPoint = M::TopRightCorner;
			LeftPoint = M::TopLeftCorner;

			RightPoint.y = RightPoint.y += (TextPos.y + 8);
			LeftPoint.y = RightPoint.y;

			Drawing::DrawTextA("Private Build", TextPos.x, TextPos.y, Colours::Green, DT_CENTER);
			Drawing::DrawLine(RightPoint, LeftPoint, M::OutlineThickness, D3DMenu::Settings::Colour::Outline);
		}
	}

	namespace Items
	{
		namespace M = D3DMenu::Settings::Menu;
		namespace C = D3DMenu::Settings::Colour;		
		namespace I = D3DMenu::Settings::Items;

		void GetInput()
		{
			// if menu is open
			if (Config::bMenuOpen)
			{
				if (menuItems[I::TotalItemCount].isTitle)
				{
					I::CurrentMenuItem++;
				}

				// preventing any overflow issues 
				if (I::CurrentMenuItem >= I::TotalItemCount)
				{
					I::CurrentMenuItem = 1;
				}
				else if (I::CurrentMenuItem < 0)
				{
					I::CurrentMenuItem = (I::TotalItemCount - 1);
				}

				// navigating menu
				if (GetAsyncKeyState(VK_DOWN) & 1)
				{
					if (menuItems[I::CurrentMenuItem + 1].isTitle || I::CurrentMenuItem + 1 == I::TotalItemCount)
					{
						if (I::CurrentMenuItem + 1 == I::TotalItemCount)
						{
							I::CurrentMenuItem = 1;
						}
						else
						{
							I::CurrentMenuItem += 2;
						}
					}
					else
					{
						I::CurrentMenuItem++;
					}
					//I::CurrentMenuItem++;
				}

				if (GetAsyncKeyState(VK_UP) & 1)
				{
					// checking to see if next element is title
					// if true check allows selected item to change
					// otherwise the current selected item won't change
					if (menuItems[I::CurrentMenuItem - 1].isTitle)
					{
						if ((I::CurrentMenuItem - 1) == 0)
						{
							I::CurrentMenuItem += I::TotalItemCount - 2;
						}
						else
						{
							I::CurrentMenuItem -= 2;
						}
					}
					else
					{
						I::CurrentMenuItem--;
					}
				}

				// toggle selected menu item
				if (GetAsyncKeyState(VK_RIGHT) & 1 || GetAsyncKeyState(VK_LEFT) & 1)
				{
					*menuItems[I::CurrentMenuItem].enabled = !(*menuItems[I::CurrentMenuItem].enabled); // a little toggle
				}
				std::cout << I::CurrentMenuItem << "\n";
				std::cout << I::TotalItemCount << "\n";
			}
		}

		// default toggled not being used
		// implimented in future
		void Add(const char* name, bool* config, bool defaultToggled, bool isTitle, int title)
		{			
			D3DMenu::menuItems[I::TotalItemCount].name = name;
			D3DMenu::menuItems[I::TotalItemCount].enabled = config;
			D3DMenu::menuItems[I::TotalItemCount].defaultToggled = defaultToggled;
			D3DMenu::menuItems[I::TotalItemCount].isTitle = isTitle;
			D3DMenu::menuItems[I::TotalItemCount].title = title;
			I::TotalItemCount++;
		}

		void Draw()
		{
			static Vec2 currentItemPos;			
			D3DCOLOR textColour;
			for (int i = 0; i < I::TotalItemCount; i++)
			{
				currentItemPos.y = (i == 0 ? I::StartingYPos : currentItemPos.y += I::Padding);
				if (menuItems[i].isTitle)
				{
					currentItemPos.x = I::TitleXPos;
					textColour = C::Title;
					if (i == menuItems[I::CurrentMenuItem].title)
					{
						textColour = C::ActiveTitle;
					}
				}
				else
				{
					currentItemPos.x = I::ItemXPos;
					textColour = i == I::CurrentMenuItem ? C::ActiveItem : C::Item;
					// bool
					Drawing::DrawTextA(*(menuItems[i].enabled) ? "ON" : "OFF", M::Position.x * 10, currentItemPos.y, textColour, DT_CENTER);
				}
				// name
				Drawing::DrawTextA(menuItems[i].name, currentItemPos.x, currentItemPos.y, textColour, DT_LEFT);
			}
		}
	}

	void Draw()
	{		
		namespace M = D3DMenu::Settings::Menu;
		namespace I = D3DMenu::Settings::Items;
		namespace C = D3DMenu::Settings::Colour;
		static bool initialised = false;

		// menu starting positions
		M::Position.x = 20;
		M::Position.y = 10;

		// menu title and item x positioning
		I::TitleXPos = M::Position.x + 10;
		I::ItemXPos = M::Position.x + 15;

		// vertices information
		// top right
		M::TopRightCorner.x = M::Position.x;
		M::TopRightCorner.y = M::Position.y;
		// top left
		M::TopLeftCorner.x = M::Position.x + M::Width;
		M::TopLeftCorner.y = M::Position.y;
		// bottom right
		M::BottomRightCorner.x = M::Position.x;
		M::BottomRightCorner.y = M::Position.y + M::Height;
		// bottom left
		M::BottomLeftCorner.x = M::Position.x + M::Width;
		M::BottomLeftCorner.y = M::Position.y + M::Height;

		// main menu rect
		Drawing::DxRect(M::Position.x, M::Position.y, M::Width, M::Height, Colours::Black);
		// top right -> top left
		Drawing::DrawLine(M::TopRightCorner, M::TopLeftCorner, M::OutlineThickness, C::Outline);
		// top left -> bottom left
		Drawing::DrawLine(M::TopLeftCorner, M::BottomLeftCorner, M::OutlineThickness, C::Outline);
		// top right -> bottom right
		Drawing::DrawLine(M::TopRightCorner, M::BottomRightCorner, M::OutlineThickness, C::Outline);
		// bottom right -> bottom left
		Drawing::DrawLine(M::BottomRightCorner, M::BottomLeftCorner, M::OutlineThickness, C::Outline);

		Header::Draw();
		Items::Draw();
		// keyboard input for menu items
		Items::GetInput();
		
		// drawing menu items once
		if (!initialised)
		{
			Items::Add("Visuals", false, false, true, 0);
			Items::Add("chams", &Config::bVisuals, false, false, 0);
			Items::Add("Misc", &Config::bVisuals, false, false, 0);
			Items::Add("dsds", false, false, true, 0);
			Items::Add("chams", &Config::bVisuals2, false, false, 3);
			Items::Add("chams", &Config::bVisuals2, false, false, 3);
			Items::Add("chams", &Config::bVisuals2, false, false, 3);
			Items::Add("chams", &Config::bVisuals2, false, false, 3);
			Items::Add("dddddsds", false, false, true, 0);
			Items::Add("chams", &Config::bVisuals2, false, false, 8);
			Items::Add("chams", &Config::bVisuals2, false, false, 8);
			Items::Add("chams", &Config::bVisuals2, false, false, 8);
			Items::Add("chams", &Config::bVisuals2, false, false, 8);
			initialised = true;
		}
	}
}
