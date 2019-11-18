// Meldown Framework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Framework.h"
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
	auto* framework = new Meltdown::Core::Framework();
	auto ecs = framework->GetECSManager();
	for (int i = 0; i < 2000; ++i)
	{
		auto& ent = ecs.AddEntity();
		ecs.AddComponent<Position>(ent, 1, 2);
		//ecs.AddComponent<Rotation>(ent, 3, 4);
		//std::cout << ecs.GetComponent<Position>(ent)->x << " : " << ecs.GetComponent<Position>(ent)->y << "\n";

	}
	system("pause");
	std::function<void(Position&)> f = [](Position& pos) { pos.x = 5; };
	for (int i = 0; i < 5; ++i)
	{ 
		const auto start = std::chrono::high_resolution_clock::now();
		ecs.ForEach<Position>(f);
		ecs.ForEach<Position>(f);
		//auto test1 = ecs.GetComponentTuples<Position>();
		//auto test2 = ecs.GetComponentTuples<Position>();
		const auto end = std::chrono::high_resolution_clock::now();
		const auto diff = end - start;
		std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(diff).count() << " ms" << std::endl;

		//std::cout << std::get<Position&>(test[4]).x << " : " << std::get<Rotation&>(test[4]).y << std::endl;
	}
	system("pause");

	delete framework;

}
