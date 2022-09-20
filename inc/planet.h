#pragma once
#include "camera.h"
#include <cmath>

using namespace mt;

namespace edit
{
	class Planet
	{
	public:
		Planet(double rad);
		Planet(double rad, double Dteta);

		~Planet();

		void Move(double dist, double angle);
		Point GetPoint(int i);
		int GetSize();

	private:
		Point* m_points = nullptr;
		double m_radius = 0;
		double m_distance = 0;
		double m_angle = 0;
		int m_size = 0;
	};
}
