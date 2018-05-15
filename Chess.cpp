// Chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"SFML\Graphics.hpp"
#include<iostream>
#include<windows.h>
#include<vector>

//Shortcuts
#define LOG(x) std::cout<<x<<std::endl
#define LOG2(x,y) std::cout<<x<<" "<<y<<std::endl


//ChessMen Defined Character

#define KING 'K'
#define QUEEN 'Q'
#define ROOK 'R'
#define BISHOP 'B'
#define KNIGHT 'N'
#define PAWN 'P'
#define UNDEFINED -1
#define EMPTY '.'


//containers
struct Window
{
	int width;
	int height;
	Window(int w,int h) :width(w), height(h) {}
	Window(){}
};

struct Mouse
{
	int x;
	int y;
	bool isLeftButtonPressed = false;
	bool isRightButtonPressed = false;
	int Quadrantx;
	int Quadranty;
	bool OOB = false;	//out of Bounds
};

//Utility Functions
namespace Utility
{
	char ToLower(char c)
	{
		char ret = c + 32;
		return ret;
	}

	char ToUpper(char c)
	{
		char ret = c - 32;
		return ret;
	}

	bool isLower(char c)
	{
		if (int(c) >= 96)
			return true;
		return false;
	}

	template<class data>
	bool BelongsTo(data a, data *Container,int container_size)
	{
		for (int i = 0; i < container_size; i++)
		{
			if (a == Container[i])
			{
				return true;
			}
		}
		return false;
	}
}


class Logic_Board
{
protected:
	char Map[8][8];		//Chess Board Location Matrix
	int selectorx = UNDEFINED;
	int selectory = UNDEFINED;
	bool selection_mode = false;
	bool move_set_i = false;
	int move_set_size=0;
public:
	Logic_Board()
	{
		reset();
	}

	void reset()
	{
		//clear Map
		for(int i=0;i<8;i++)
		{
			for (int j = 0;j<8;j++)
			{
				Map[i][j] = EMPTY;
			}
		}

		char kingdom[] = { ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK };
		for (int i = 0; i < 8; i++)
		{
			Map[7][i] = kingdom[i];
			Map[0][i] = Utility::ToLower(kingdom[i]);
			Map[1][i] = Utility::ToLower(PAWN);
			Map[6][i] = PAWN;
		}
	}

	std::vector<std::pair<float,float>> get_move_set(int x,int y)
	{
		if (Map[x][y] == KING)
		{
			return get_moves_King(x, y, true);
		}
		else if (Map[x][y] == Utility::ToLower(KING))
		{
			return get_moves_King(x, y, false);
		}
		else if (Map[x][y] == QUEEN)
		{
			return get_moves_Queen(x, y, true);
		}
		else if (Map[x][y] == Utility::ToLower(QUEEN))
		{
			return get_moves_Queen(x, y, false);
		}
		else if (Map[x][y] == PAWN)
		{
			return get_moves_Pawn(x, y, true);
		}
		else if (Map[x][y] == Utility::ToLower(PAWN))
		{
			return get_moves_Pawn(x, y, false);
		}
		else if (Map[x][y] == BISHOP)
		{
			return get_moves_Bishop(x, y, true);
		}
		else if (Map[x][y] == Utility::ToLower(BISHOP))
		{
			return get_moves_Bishop(x, y, false);
		}
		else if (Map[x][y] == ROOK)
		{
			return get_moves_Rook(x, y, true);
		}
		else if (Map[x][y] == Utility::ToLower(ROOK))
		{
			return get_moves_Rook(x, y, false);
		}
		else if (Map[x][y] == KNIGHT)
		{
			return get_moves_Knight(x, y, true);
		}
		else if (Map[x][y] == Utility::ToLower(KNIGHT))
		{
			return get_moves_Knight(x, y, false);
		}
	}

	std::vector<std::pair<float, float>> get_moves_King(int x, int y,bool isWhite)
	{
		std::vector<std::pair<float, float>> ret;
		//Rectangle
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{

				if (!(i == x && j == y) && i>=0 && i<=7 && j>=0 && j<=7)
				{
					if (Map[i][j] == EMPTY)
					{
						ret.push_back(std::make_pair(i, j));
					}
					else
					{
						if (isWhite)	//if the selection is white
						{
							if (!Utility::isLower(Map[i][j]))	//if adjacent cell is also white
							{
								//do nothing
							}
							else
							{
								ret.push_back(std::make_pair(i, j));
							}
						}
						else //if selection is black
						{
							if (Utility::isLower(Map[i][j]))	//if adjacent cell is also black
							{
								//do nothing
							}
							else
							{
								ret.push_back(std::make_pair(i, j));
							}
						}
					}
				}
			}
		}

		move_set_size = ret.size();
		return ret;
	}
	std::vector<std::pair<float, float>> get_moves_Queen(int x, int y, bool isWhite)
	{
		std::vector<std::pair<float, float>> ret;
		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Rook(int x, int y, bool isWhite)
	{
		std::vector<std::pair<float, float>> ret;
		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Knight(int x, int y, bool isWhite)
	{
		std::vector<std::pair<float, float>> ret;
		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Bishop(int x, int y, bool isWhite)
	{
		std::vector<std::pair<float, float>> ret;
		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Pawn(int x, int y, bool isWhite)
	{
		std::vector<std::pair<float, float>> ret;
		return ret;
	}



	

		void printMap()
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					std::cout << Map[i][j] << "  ";
				}
				std::cout << std::endl;
			}
		}
		
};


//Abstract Class for the Chessmen
class Chessmen				
{
protected:
	int move_count;
	int x;
	int y;
	bool isAlive = true;
	virtual void move() = 0;
	
	void set_position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

};

class King:public Chessmen
{

};
class Queen:public Chessmen
{

};
class Rook:public Chessmen
{

};
class Bishop:public Chessmen
{

};
class Knight:public Chessmen
{

};
class Pawn:public Chessmen
{

};

class Elements :public King, public Queen, public Rook, public Bishop, public Knight, public Pawn
{

};


class Graphics
{
protected:

	//Textures
	sf::Texture Board_Texture;
	//Chess Pieces Textures
	sf::Texture texture_b1, texture_b2, texture_k1, texture_k2, texture_n1, texture_n2, texture_p1, texture_p2, texture_q1, texture_q2, texture_r1, texture_r2;

	sf::Sprite ChessBoard;
	sf::Sprite bishop1, bishop2, king1, king2, knight1, knight2, pawn1, pawn2, queen1, queen2, rook1, rook2;


	sf::RectangleShape highlighter;
	sf::CircleShape circle;
	void load_textures()
	{
		//Load Textures into Memory
		Board_Texture.loadFromFile("Images/board.png");
		texture_b1.loadFromFile("Images/bishop_1.png");
		texture_b2.loadFromFile("Images/bishop_2.png");
		texture_k1.loadFromFile("Images/king_1.png");
		texture_k2.loadFromFile("Images/king_2.png");
		texture_n1.loadFromFile("Images/knight_1.png");
		texture_n2.loadFromFile("Images/knight_2.png");
		texture_p1.loadFromFile("Images/pawn_1.png");
		texture_p2.loadFromFile("Images/pawn_2.png");
		texture_q1.loadFromFile("Images/queen_1.png");
		texture_q2.loadFromFile("Images/queen_2.png");
		texture_r1.loadFromFile("Images/rook_1.png");
		texture_r2.loadFromFile("Images/rook_2.png");

		//Set Textures to Spirtes
		ChessBoard.setTexture(Board_Texture);
		bishop1.setTexture(texture_b1);
		bishop2.setTexture(texture_b2);
		king1.setTexture(texture_k1);
		king2.setTexture(texture_k2);
		knight1.setTexture(texture_n1);
		knight2.setTexture(texture_n2);
		pawn1.setTexture(texture_p1);
		pawn2.setTexture(texture_p2);
		queen1.setTexture(texture_q1);
		queen2.setTexture(texture_q2);
		rook1.setTexture(texture_r1);
		rook2.setTexture(texture_r2);
	}
};


class Game:public Graphics,public Logic_Board
{
private:
	sf::RenderWindow *app;
	Mouse mouse;
	Window window;
	std::vector<std::pair<float, float>>  move_set;
public:
	Game(const Window win)
	{
		window.width = win.width;
		window.height = win.height;
		sf::RenderWindow *applet = new sf::RenderWindow(sf::VideoMode(window.width, window.height), "Chess");
		app = applet;

		init();
	}
private:
	void init()
	{
		load_textures();


		highlighter.setSize(sf::Vector2f(window.width / 8, window.height / 8));
		highlighter.setOutlineColor(sf::Color::Red);
		highlighter.setOutlineThickness(2);
		highlighter.setFillColor(sf::Color(0, 0, 0, 0));

		circle.setRadius(window.width / 32);
		circle.setOrigin(window.width / 16, window.width / 16);

		loop();
	}

	void loop()
	{
		while (app->isOpen())
		{
			app->clear();

			manage_events();

			draw();
			

			update();

			app->display();

		}
	}

	void draw()
	{
		app->draw(ChessBoard);
		//draw the chess pieces at the respective position
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (Map[i][j] == BISHOP || Map[i][j] == Utility::ToLower(BISHOP))
					{
						if (Utility::isLower(Map[i][j]))	//draw black i.e 2
						{
							bishop2.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(bishop2);
						}
						else								//draw white i.e 1
						{
							bishop1.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(bishop1);
						}
					}
					else if (Map[i][j] == KING || Map[i][j] == Utility::ToLower(KING))
					{
						if (Utility::isLower(Map[i][j]))	//draw black i.e 2
						{
							king2.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(king2);
						}
						else								//draw white i.e 1
						{
							king1.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(king1);
						}
					}
					else if (Map[i][j] == QUEEN || Map[i][j] == Utility::ToLower(QUEEN))
					{
						if (Utility::isLower(Map[i][j]))	//draw black i.e 2
						{
							queen2.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(queen2);
						}
						else								//draw white i.e 1
						{
							queen1.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(queen1);
						}
					}
					else if (Map[i][j] == ROOK || Map[i][j] == Utility::ToLower(ROOK))
					{
						if (Utility::isLower(Map[i][j]))	//draw black i.e 2
						{
							rook2.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(rook2);
						}
						else								//draw white i.e 1
						{
							rook1.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(rook1);
						}
					}
					else if (Map[i][j] == KNIGHT || Map[i][j] == Utility::ToLower(KNIGHT))
					{
						if (Utility::isLower(Map[i][j]))	//draw black i.e 2
						{
							knight2.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(knight2);
						}
						else								//draw white i.e 1
						{
							knight1.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(knight1);
						}
					}
					else if (Map[i][j] == PAWN || Map[i][j] == Utility::ToLower(PAWN))
					{
						if (Utility::isLower(Map[i][j]))	//draw black i.e 2
						{
							pawn2.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(pawn2);
						}
						else								//draw white i.e 1
						{
							pawn1.setPosition(sf::Vector2f(125 * j + 10, 125 * i + 10));
							app->draw(pawn1);
						}
					}

				}
			}
		}

		//Draw Highlights
		{
			if (selection_mode)
			{
				app->draw(highlighter);            
			}
		}

		//draw Move 
		if(move_set_i)
		{

			int iterator = 0;
			while (iterator < move_set.size())
			{

				int xpos = move_set[iterator].first;
				int ypos = move_set[iterator].second;

				circle.setPosition(ypos*window.height / 8 + window.height / 12, xpos*window.width / 8 + window.width / 12);
				if (Map[xpos][ypos] !=EMPTY)
				{
					circle.setFillColor(sf::Color::Red);
				}
				else
				{
					circle.setFillColor(sf::Color::Green);
				}
				app->draw(circle);
				iterator++;
			}
		}
	}

	void manage_events()
	{
		sf::Event event;
		while (app->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				app->close();
			}
		}

		//Mouse Events
		{
			sf::Vector2i m = sf::Mouse::getPosition(*app);
			if (m.x <= window.width && m.x >= 0 && m.y <= window.height && m.y >= 0)
			{
				mouse.OOB = false;
				mouse.x = m.x;
				mouse.y = m.y;
			}
			else
			{
				mouse.OOB = true;
			}
			mouse.isLeftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			mouse.isRightButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
			
			//Convert the Mouse cordinates to the Board Quadrants
			mouse.Quadrantx =(float) mouse.x / window.width * 8;
			mouse.Quadranty = (float)mouse.y / window.height * 8;
		}

	}

	void update()
	{
		select_piece();
		
	}

	void select_piece()//Select Matrix Piece
	{
		if (mouse.isLeftButtonPressed && !mouse.OOB && Map[mouse.Quadranty][mouse.Quadrantx] != EMPTY && selectorx == UNDEFINED && selectory == UNDEFINED)	//Selected
		{
			selection_mode = true;
			move_set_i = true;
			selectorx = mouse.Quadrantx;
			selectory = mouse.Quadranty;
			highlighter.setPosition(window.width / 8 * selectorx, window.height / 8 * selectory);
			load_move_set();
			Sleep(100);
		}
		else if (mouse.isLeftButtonPressed && selectorx != UNDEFINED && selectory != UNDEFINED && !  (mouse.Quadrantx == selectorx && mouse.Quadranty==selectory))	//Transmitted
		{
			if (verify_move(mouse.Quadrantx,mouse.Quadranty) || true)
			{
				Map[mouse.Quadranty][mouse.Quadrantx] = Map[selectory][selectorx];
				Map[selectory][selectorx] = EMPTY;
				selectorx = UNDEFINED;
				selectory = UNDEFINED;
				selection_mode = false;
				move_set_i = false;
				Sleep(100);
			}
		}

		if (mouse.isRightButtonPressed)
		{
			selectorx = UNDEFINED;
			selectory = UNDEFINED;
			selection_mode = false;
			move_set_i = false;
			Sleep(100);
		}
	}

	bool verify_move(int tox,int toy)
	{
		for (int i = 0; i < move_set.size(); i++)
		{
			if (toy == move_set[i].first && tox == move_set[i].second)
			{
				LOG("Valid Move");
				return true;
			}
		}
		LOG("Invalid Move");
		return false;
	}

	void load_move_set()
	{
		move_set =  get_move_set(selectory, selectorx);
	}

};

int main()
{
	Window window(1000, 1000);
	Game game(window);

    return 0;
}

