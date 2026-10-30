#pragma once
#include "grid.h"
#include <iostream>

#define RED "\033[41m"
#define WHITE "\033[47m"
#define White "\033[0;37m"

#define ENTER 13
#define ESCAPE 27

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
	void BFS(Node* start, Node* target);
	void Djikstra(Node* start, Node* target);
	void InitObstacle();

    int partition(std::vector<int>& vec, int low, int high);
    void quickSort(std::vector<int>& vec, int low, int high);

private:

	std::vector<Node*> path;

	Grid grid;
	Player p1;
	Cursor c1;

	int deplacement = 0;

    int cost = 0;

	bool attainable = true;
    bool ActivateCost = false;
};
