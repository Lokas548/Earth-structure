#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;
namespace mt
{
	struct Point
	{
		double x, y, z;
	};

	struct Angles
	{
		double roll, pitch;
	};

	struct Pixel
	{
		uint8_t r, g, b, a;
	};

	struct Intrinsic
	{
		double fu, fv;
		double du, dv;
	};

	class Camera
	{
	public:
		Camera(int width, int height, Intrinsic intrinsic, Point position, Angles angles);
		~Camera();

		Pixel* Picture();
		void Fill(Pixel pixel);
		void Clear();

		void ProjectPoint(Point p, Pixel c);

		void RotateCamera(Vector2i& LastMousPos, unique_ptr<Camera>& m_camera, unique_ptr<RenderWindow>& m_window, double dangle);
		void MoveCamera(unique_ptr<Camera>& m_camera, double kof);

		void dX(double d);
		void dZ(double d);
		void dRoll(double droll);
		void dPitch(double dpitch);

	private:
		int m_width;
		int m_height;

		Pixel* m_picture;

		Intrinsic m_intrinsic;

		Point m_position;
		Angles m_angles;
		float m_rad;
	};
}