#pragma once
#include "grid.h"
#include <iostream>
#include <vector>

namespace Ansi
{
    constexpr const char* Red = "\033[41m";
    constexpr const char* WhiteBg = "\033[47m";
    constexpr const char* White = "\033[0;37m";
}

constexpr int KEY_ENTER = 13;
constexpr int KEY_ESCAPE = 27;

struct Player
{
	int xp = 0;
	int yp = 0;

	//Get
	int GetxP() { return xp; }
	int GetyP() { return yp; }

	//Set
	void SetxP(int value) { xp = value; }
	void SetyP(int value) { yp = value; }
};

struct Cursor
{
	int xc = 0;
	int yc = 0;

	//Get
	int GetxC() { return xc; }
	int GetyC() { return yc; }

	//Set
	void SetxC(int value) { xc = value; }
	void SetyC(int value) { yc = value; }
};

class App
{
public:

	App();
	~App();

	void Update();
	void Draw();
	void DrawGrid();
	void HandleInput();
	//void InitObstacle();
    void BFS(Node* start, Node* target);                  // grille interne (console)
    void BFS(Grid& g, Node* start, Node* target);         // grille au choix
    void Djikstra(Node* start, Node* target);
    void Djikstra(Grid& g, Node* start, Node* target);

    int  partition(std::vector<int>& vec, int low, int high);
    void quickSort(std::vector<int>& vec, int low, int high);

    bool GetAttainable() { return attainable; }
    const std::vector<Node*>& GetPath() const { return path; }
    int  GetCost() const { return cost; }

private:

	std::vector<Node*> path;

	Grid grid;
	Player p1;
	Cursor c1;

	int deplacement = 0;

    int cost = 0;

	bool attainable = true;
    bool ActivateCost = true;
};
