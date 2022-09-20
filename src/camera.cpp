#include "camera.h"
#include "matrix.h"

namespace mt
{
	using math::Mat33d;
	using math::Vec3d;

	Camera::Camera(int width, int height, Intrinsic intrinsic, Point position, Angles angles)
	{
		m_height = height;
		m_width = width;
		m_intrinsic = intrinsic;
		m_picture = new Pixel[m_width * m_height];
		m_position = position;
		m_angles = angles;

		Clear();
	}

	Camera::~Camera()
	{
		delete[] m_picture;
	}

	Pixel* Camera::Picture() { return m_picture; }

	void Camera::Fill(Pixel pixel)
	{
		for (int i = 0; i < m_height; i++)
			for (int j = 0; j < m_width; j++)
				m_picture[i * m_width + j] = pixel;
	}

	void Camera::Clear()
	{
		Fill({ 0,0,0,0 });
	}

	void Camera::RotateCamera(Vector2i& LastMousPos, unique_ptr<Camera>& m_camera, unique_ptr<RenderWindow>& m_window, double dangle)
	{
		Vector2i MousePos = Mouse::getPosition(*m_window);

		float Dx = MousePos.x - LastMousPos.x;
		float Dy = MousePos.y - LastMousPos.y;

		if (Dx != 0)
		{
			m_camera->dPitch(Dx * dangle);
			Mouse::setPosition({ m_width / 2,m_height / 2 }, *m_window);
		}
		if (Dy != 0)
		{
			m_camera->dRoll(Dy * dangle);
			Mouse::setPosition({ m_width / 2,m_height / 2 }, *m_window);
		}

		LastMousPos = MousePos;
	}

	void Camera::MoveCamera(unique_ptr<Camera>& m_camera, double kof)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			m_camera->dZ(kof);
		}
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))
		{
			m_camera->dZ(3 * kof);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			m_camera->dZ(-kof);
		}
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))
		{
			m_camera->dZ(-3 * kof);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			m_camera->dX(-kof);
		}
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))
		{
			m_camera->dX(-3 * kof);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_camera->dX(kof);
		}
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)))
		{
			m_camera->dX(3 * kof);
		}

	}

	void Camera::ProjectPoint(Point p, Pixel c)
	{
		// Camera tilt 90 degrees
		double X = p.x;
		double Y = -p.z;
		double Z = p.y;

		double a = m_angles.roll;
		double b = m_angles.pitch;

		double cosa = cos(a);
		double cosb = cos(b);
		double sina = sin(a);
		double sinb = sin(b);

		Mat33d R({ {
						  {cosb, 0, -sinb},
						  {-sina * sinb, cosa, -sina * cosb},
						  {cosa * sinb, sina, cosa * cosb}
				  } });

		Vec3d P({ {
						 {X - m_position.x},
						 {Y - m_position.y},
						 {Z - m_position.z}
				 } });

		Vec3d P_res;

		P_res = R * P;

		double x = P_res.get(0, 0);
		double y = P_res.get(1, 0);
		double z = P_res.get(2, 0);

		// the object is behind the camera
		if (z <= 0)
			return;

		// Perspective projection (3D->2D)
		double u = x / z;
		double v = y / z;

		// Apply inside camera settings
		u = u * m_intrinsic.fu + m_intrinsic.du;
		v = v * m_intrinsic.fv + m_intrinsic.dv;

		if (u >= 0 && u < m_width && v >= 0 && v < m_height)
		{
			m_picture[(int)v * m_width + (int)u] = c;
		}
	}

	void Camera::dX(double d)
	{
		m_position.x += d * cos(-m_angles.pitch);
		m_position.y += 0;
		m_position.z += d * sin(-m_angles.pitch);
	}

	void Camera::dZ(double d)
	{
		m_position.x += d * sin(m_angles.pitch);
		m_position.y += d * sin(m_angles.roll);
		m_position.z += d * cos(m_angles.pitch);
	}

	void Camera::dRoll(double droll)
	{
		m_angles.roll += droll;
	}

	void Camera::dPitch(double dpitch)
	{
		m_angles.pitch += dpitch;
	}
}