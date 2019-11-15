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
	for (int i = 0; i < 150; ++i)
	{
		auto& ent = ecs.AddEntity();
		ecs.AddComponent<Position>(ent, i, 2 * i);
		ecs.AddComponent<Rotation>(ent, 14, 2424);
		//std::cout << ecs.GetComponent<Position>(ent)->x << " : " << ecs.GetComponent<Position>(ent)->y << "\n";

	}
	system("pause");
	std::function<void(Position&, Rotation&)> f = [](Position& pos, Rotation& rot) { };
	for (int i = 0; i < 5; ++i)
	{ 
		const auto start = std::chrono::high_resolution_clock::now();
		ecs.ForEach<Position, Rotation>(f);
		//auto test = *ecs.GetComponentTuples<Position, Rotation>();
		const auto end = std::chrono::high_resolution_clock::now();
		const auto diff = end - start;
		std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;

		//std::cout << std::get<Position&>(test[4]).x << " : " << std::get<Rotation&>(test[4]).y << std::endl;
	}
	system("pause");

	delete engine;

}
