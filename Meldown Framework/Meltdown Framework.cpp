// Meldown Framework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
#include "ECSManager.h"
#include "chrono"

struct Position
{
	Position(double a, double b):x(a),y(b){}
	double x, y;
};
struct Rotation
{
	Rotation(double a, double b) :x(a), y(b) {}
	double x, y;
};

int main()
{
	auto* engine = new Meltdown::Core::Engine();
	auto ecs = engine->GetECSManager();
	for (int i = 0; i < 1000; ++i)
	{
		auto ent = ecs.AddEntity();
		ecs.AddComponent<Position>(ent, i, 2 * i);
		ecs.AddComponent<Rotation>(ent, 14, 2424);
		//std::cout << ecs.GetComponent<Position>(ent)->x << " : " << ecs.GetComponent<Position>(ent)->y << "\n";
		//std::cout << ecs.HasComponents<Position, Rotation>(ent) << "\n";
	}
	system("pause");
	auto start = std::chrono::high_resolution_clock::now();
	auto query1 = *ecs.GetComponentTuples<Rotation, Position>();
	auto query2 = *ecs.GetComponentTuples<Position>();
	auto query3 = *ecs.GetComponentTuples<Rotation>();
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	std::cout << std::chrono::duration <double, std::micro>(diff).count() << " micro" << std::endl;
	system("pause");


	delete engine;

}
