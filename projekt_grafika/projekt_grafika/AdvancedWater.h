#pragma once
#include "SFML/Graphics.hpp"

class WaterParticle : public sf::Drawable{
	friend class WaterGroup;
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float squareDistance(sf::Vector2f point);
	void updateVelocity();
	void updatePosition();
	void checkColisions(std::vector <sf::Vector2f>);
	int m_x;
	int m_y;
	int m_r;
	int m_x_velocity;
	int m_y_velocity;
};

