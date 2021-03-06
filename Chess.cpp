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
	template<typename var>
	void swap(var &x, var &y)
	{
		var c = x;
		x = y;
		y = c;
	}
	char ToLower(const char c)
	{
		char ret = c + 32;
		return ret;
	}

	char ToUpper(const char c)
	{
		char ret = c - 32;
		return ret;
	}

	bool isLower(const char c)
	{
		if (int(c) >= 96)
			return true;
		return false;
	}

	bool isBlack(const char t)
	{
		if (isLower(t))
			return true;
		return false;
	}

	bool isWhite(const char t)
	{
		if (isLower(t))
			return false;
		return true;
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
	
	bool in_limits(const int x,const int y)
	{
		if (x >= 0 && y >= 0 && x <= 7 && y <= 7)
			return true;
		return false;
	}

	bool is_it_ok_to_push_at(bool &conditioner ,const int ty, const int tx,const char cell, const bool self)
	{
		bool ret = false;
		if (conditioner)
		{
			if (!in_limits(tx,ty))
			{
				conditioner = false;
				ret = false;
			}
			else
			{
				if ((isWhite(cell) != self) || cell == EMPTY)
				{
					ret = true;
				}
				else
				{
					conditioner = false;
					ret = false;
				}
				if (cell != EMPTY)
				{
					conditioner = false;
					if (self == isWhite(cell))
					{
						ret = false;
					}
					else
					{
					ret = true;

					}
				}
			}
		}
		return ret;
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
		if (Map[x][y] == KING || Map[x][y] == Utility::ToLower(KING))
		{
			return get_moves_King(x, y, Utility::isWhite(Map[x][y]));
		}
		else if (Map[x][y] == QUEEN || Map[x][y] == Utility::ToLower(QUEEN))
		{
			return get_moves_Queen(x, y, Utility::isWhite(Map[x][y]));
		}
		else if (Map[x][y] == PAWN || Map[x][y] == Utility::ToLower(PAWN))
		{
			return get_moves_Pawn(x, y, Utility::isWhite(Map[x][y]));
		}
		else if (Map[x][y] == BISHOP || Map[x][y] == Utility::ToLower(BISHOP))
		{
			return get_moves_Bishop(x, y, Utility::isWhite(Map[x][y]));
		}
		else if (Map[x][y] == ROOK || Map[x][y] == Utility::ToLower(ROOK))
		{
			return get_moves_Rook(x, y, Utility::isWhite(Map[x][y]));
		}
		else if (Map[x][y] == KNIGHT || Map[x][y] == Utility::ToLower(KNIGHT))
		{
			return get_moves_Knight(x, y, Utility::isWhite(Map[x][y]));
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
							if (Utility::isWhite(Map[i][j]))	//if adjacent cell is also white
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
							if (Utility::isBlack(Map[i][j]))	//if adjacent cell is also black
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

		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Queen(int x, int y, bool isWhite)
	{
		Utility::swap<int>(x, y);

		std::vector<std::pair<float, float>> ret;
		
		bool n, s, e, w, ne, nw, se, sw ;	//directions North-south-east-west..etc
		n = s = e = w = ne = nw = se = sw = true;
		int inum = 1;
		while (n||s||e||w||ne||nw||se||sw)
		{
			
			//North Direction
			if (n)
			{
				int ty = y - inum;
				int tx = x;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(n, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					n = false;
				}
			}
			
			//South Direction
			if (s)
			{
				int ty = y + inum;
				int tx = x;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(s, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					s = false;
				}
			}
			
			//East Direction
			if (e)
			{
				int ty = y ;
				int tx = x + inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(e, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					e = false;
				}
			}
			//West Direction
			if (w)
			{
				int ty = y ;
				int tx = x -inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(w, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					w = false;
				}
			}

			//North-East Direction
			if (ne)
			{
				int ty = y - inum;
				int tx = x +inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(ne, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					ne = false;
				}
			}
			//North-West Direction
			if (nw)
			{
				int ty = y - inum;
				int tx = x-inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(nw, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					nw = false;
				}
			}
			//South-East Direction
			if (se)
			{
				int ty = y + inum;
				int tx = x +inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(se, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}	
				}
				else
				{
					se = false;
				}
			}
			//South-West Direction
			if (sw)
			{
				int ty = y + inum;
				int tx = x -inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(sw , ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					sw = false;
				}
			}
			

			inum++;
		}

		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Rook(int x, int y, bool isWhite)
	{
		Utility::swap<int>(x, y);


		std::vector<std::pair<float, float>> ret;

		bool n, s, e, w;	//directions North-south-east-west..etc
		n = s = e = w=true;
		int inum = 1;
		while (n || s || e || w)
		{

			//North Direction
			if (n)
			{
				int ty = y - inum;
				int tx = x;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(n, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					n = false;
				}
			}

			//South Direction
			if (s)
			{
				int ty = y + inum;
				int tx = x;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(s, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					s = false;
				}
			}

			//East Direction
			if (e)
			{
				int ty = y;
				int tx = x + inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(e, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					e = false;
				}
			}
			//West Direction
			if (w)
			{
				int ty = y;
				int tx = x - inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(w, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					w = false;
				}
			}

			inum++;
		}



		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Bishop(int x, int y, bool isWhite)
	{
		Utility::swap<int>(x, y);

		std::vector<std::pair<float, float>> ret;

		bool ne, nw, se, sw;	//directions North-south-east-west..etc
		ne = nw = se = sw = true;
		int inum = 1;
		while (ne || nw || se || sw)
		{
			//North-East Direction
			if (ne)
			{
				int ty = y - inum;
				int tx = x + inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(ne, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					ne = false;
				}
			}
			//North-West Direction
			if (nw)
			{
				int ty = y - inum;
				int tx = x - inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(nw, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					nw = false;
				}
			}
			//South-East Direction
			if (se)
			{
				int ty = y + inum;
				int tx = x + inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(se, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					se = false;
				}
			}
			//South-West Direction
			if (sw)
			{
				int ty = y + inum;
				int tx = x - inum;
				if (Utility::in_limits(tx, ty))
				{
					char cell = Map[ty][tx];
					if (Utility::is_it_ok_to_push_at(sw, ty, tx, cell, isWhite))
					{
						ret.push_back(std::make_pair(ty, tx));
					}
				}
				else
				{
					sw = false;
				}
			}


			inum++;
		}



		return ret;
	}

	std::vector<std::pair<float, float>> get_moves_Knight(int x, int y, bool isWhite)
	{
		Utility::swap<int>(x, y);

		std::vector<std::pair<float, float>> ret;

		int ty, tx;

		//The Following Set COntains All the Possible Locations Relative to the Knight
		//  North-e,North-w,East-n,East-s,South-e,South-w,West-north,West-s
		int ty_set[] = {y-2,y-2,y-1,y+1,y+2,y+2,y+1,y-1};
		int tx_set[] = { x + 1,x - 1,x + 2,x + 2,x + 1,x - 1,x - 2,x - 2 };
		
		for (int i = 0; i < 8; i++)
		{
			ty = ty_set[i];
			tx = tx_set[i];
			if (Utility::in_limits(ty, tx))
			{
				char cell = Map[ty][tx];
				if ((Utility::isBlack(cell) && isWhite) || (Utility::isWhite(cell) && !isWhite) || cell==EMPTY )//if Cell is Black and Self is White (OR ) //if Cell is White and Self is Black (OR ) Cell is EMPTY
				{
					ret.push_back(std::make_pair(ty, tx));
				}
			}

			}


		return ret;
	}


	std::vector<std::pair<float, float>> get_moves_Pawn(int x, int y, bool isWhite)
	{
		Utility::swap<int>(x, y);

		std::vector<std::pair<float, float>> ret;
		bool enemy_flag = false;

		if(isWhite)	//default for white
		{
			if (Utility::isBlack(Map[y-1][x]) || Map[y-1][x] == EMPTY)
			{
				if (Utility::isBlack(Map[y - 1][x]) && Map[y-1][x]!=EMPTY)
				{
					enemy_flag = true;
				}
				ret.push_back(std::make_pair(y - 1, x));
			}
			if (Utility::isBlack(Map[y - 1][x - 1]) && Map[y-1][x-1]!=EMPTY)
			{
				ret.push_back(std::make_pair(y - 1, x - 1));
			}
			if (Utility::isBlack(Map[y - 1][x + 1]) && Map[y - 1][x + 1]!=EMPTY)
			{
				ret.push_back(std::make_pair(y - 1, x + 1));
			}
		}
		else if (!isWhite)	//default for black
		{
			if (Utility::isWhite(Map[y+1][x]) || Map[y+1][x] == EMPTY)
			{
				if (Utility::isWhite(Map[y + 1][x])  && Map[y+1][x]!=EMPTY)
				{
					enemy_flag = true;
				}
				ret.push_back(std::make_pair(y + 1, x));
			}
			if (Utility::isWhite(Map[y + 1][x -1]) && Map[y + 1][x- 1]!=EMPTY)
			{
				ret.push_back(std::make_pair(y + 1, x - 1));
			}
			if (Utility::isWhite(Map[y + 1][x + 1]) && Map[y + 1][x + 1]!=EMPTY)
			{
				ret.push_back(std::make_pair(y + 1, x + 1));
			}
		}
		if (isWhite && y == 6 && !enemy_flag)	//initial for white
		{ 
			if (Utility::isBlack(Map[y-1][x]) || Map[y-1][x]==EMPTY)
			{
				if(Utility::isBlack(Map[y-2][x]) || Map[y-2][x] == EMPTY)
					ret.push_back(std::make_pair(y - 2, x));
			}
		}
		else if(!isWhite && y == 1 && !enemy_flag)	//initial for black
		{
			if (Utility::isWhite(Map[y+1][x]) || Map[y+1][x] == EMPTY)
			{
				if (Utility::isBlack(Map[y+2][x]) || Map[y+2][x] == EMPTY)
				{
					ret.push_back(std::make_pair(y + 2, x));
				}
			}

		}


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
		Board_Texture.loadFromFile("Spirte/board.png");
		texture_b1.loadFromFile("Spirte/bishop_1.png");
		texture_b2.loadFromFile("Spirte/bishop_2.png");
		texture_k1.loadFromFile("Spirte/king_1.png");
		texture_k2.loadFromFile("Spirte/king_2.png");
		texture_n1.loadFromFile("Spirte/knight_1.png");
		texture_n2.loadFromFile("Spirte/knight_2.png");
		texture_p1.loadFromFile("Spirte/pawn_1.png");
		texture_p2.loadFromFile("Spirte/pawn_2.png");
		texture_q1.loadFromFile("Spirte/queen_1.png");
		texture_q2.loadFromFile("Spirte/queen_2.png");
		texture_r1.loadFromFile("Spirte/rook_1.png");
		texture_r2.loadFromFile("Spirte/rook_2.png");

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

	bool whitePlays = true;
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
			if (whitePlays == Utility::isWhite(Map[mouse.Quadranty][mouse.Quadrantx]))
			{
				selection_mode = true;
				move_set_i = true;
				selectorx = mouse.Quadrantx;
				selectory = mouse.Quadranty;
				highlighter.setPosition(window.width / 8 * selectorx, window.height / 8 * selectory);
				load_move_set();
				Sleep(100);
			}
		}
		else if (mouse.isLeftButtonPressed && selectorx != UNDEFINED && selectory != UNDEFINED && !  (mouse.Quadrantx == selectorx && mouse.Quadranty==selectory))	//Transmitted
		{
			if (verify_move(mouse.Quadrantx,mouse.Quadranty))
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
				whitePlays = !whitePlays;
				return true;
			}
		}
		LOG("Invalid Move");
		mouse.isRightButtonPressed = true;
		return false;
	}

	void load_move_set()
	{
		move_set =  get_move_set(selectory, selectorx);
	}

};

int main()
{
	std::cout << "White Plays First! \n1.Left Mouse Button: Select \n2.Right Mouse Button: Cancel Selection" << std::endl;
	Window window(1000, 1000);
	Game game(window);

    return 0;
}

