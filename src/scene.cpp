#include "scene.h"
#include "planet.h"
#define PI 3.1415926535
Vector2i LastMousPos;

namespace mt
{
	Scene::Scene(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(m_width, m_height), "Earth`s Structure");
		m_texture = std::make_unique<sf::Texture>();
		m_texture->create(m_width, m_height);
		m_sprite = std::make_unique<sf::Sprite>(*m_texture);

		Intrinsic intrinsic = { 960.0, 540.0, 960.0, 540.0 };
		Point position = { 5.0, 0.0, 0.0 };
		Angles angles = { 0.0,90.0 };
		m_camera = std::make_unique<Camera>(m_width, m_height, intrinsic, position, angles);

		//models points initialisation
		m_points = new Point[2000000];
		m_points1 = new Point[200000];
		m_points2 = new Point[200000];


	}
	Scene::~Scene()
	{
		if (m_points != nullptr)
			delete[] m_points;
	}


	void Scene::LifeCycle()
	{
		double y0 = 1;

		while (m_window->isOpen()) {
			sf::Event event;
			while (m_window->pollEvent(event))
				if (event.type == sf::Event::Closed)
					m_window->close();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				m_camera->dZ(0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				m_camera->dZ(-0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_camera->dX(-0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_camera->dX(0.1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				m_camera->dPitch(-0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				m_camera->dPitch(0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				m_camera->dRoll(-0.02);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				m_camera->dRoll(0.02);
			}


			sf::Mouse::setPosition(sf::Vector2i(m_width / 2, m_height / 2), *m_window);
			double velocity = 0;
			while (m_window->isOpen())
			{
				m_camera->RotateCamera(LastMousPos, m_camera, m_window, 0.002);

				sf::Event event;
				while (m_window->pollEvent(event))
				{
					if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
						m_window->close();
				}

				m_window->setMouseCursorVisible(false);

				m_camera->MoveCamera(m_camera, 0.8);

				LastMousPos = Mouse::getPosition(*m_window);

				//earth structure creation

				m_size = 0;
				double r = 5;
				for (double fi = 0; fi < 2 * PI / 1.2; fi += 0.04)
					for (double teta = 0; teta < PI; teta += 0.04)
					{
						m_points[m_size].x = r * sin(teta) * cos(fi) / 1.5;
						m_points[m_size].y = r * sin(teta) * sin(fi) / 1.5;
						m_points[m_size].z = r * cos(teta);
						m_size++;
					}


				double r1 = 3;
				for (double fi = 0; fi < 2 * PI / 1.2; fi += 0.04)
					for (double teta = 0; teta < PI; teta += 0.04)
					{
						m_points1[m_size].x = r1 * sin(teta) * cos(fi) / 1.5;
						m_points1[m_size].y = r1 * sin(teta) * sin(fi) / 1.5;
						m_points1[m_size].z = r1 * cos(teta);
						m_size++;
					}

				double r2 = 1;
				for (double fi = 0; fi < 2 * PI / 1.2; fi += 0.04)
					for (double teta = 0; teta < PI; teta += 0.04)
					{
						m_points2[m_size].x = r2 * sin(teta) * cos(fi) / 1.5;
						m_points2[m_size].y = r2 * sin(teta) * sin(fi) / 1.5;
						m_points2[m_size].z = r2 * cos(teta);
						m_size++;
					}


				//points drawing  
				for (int i = 0; i < m_size; i++) {
					m_camera->ProjectPoint(m_points[i], { 16, 52 ,166, 255 });
					m_camera->ProjectPoint(m_points1[i], { 253, 106 ,2, 255 });
					m_camera->ProjectPoint(m_points2[i], { 255, 0 ,0, 255 });
				}

				m_texture->update((uint8_t*)m_camera->Picture(), 1920, 1080, 0, 0);
				m_camera->Clear();


				m_window->clear();
				m_window->draw(*m_sprite);

				m_window->display();

			}
		}
	}
}
