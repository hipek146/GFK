#include "AdvancedWater.h"	

void WaterParticle::draw(sf::RenderTarget& target, sf::RenderStates states) const {


}

float  WaterParticle::squareDistance(sf::Vector2f point)
{
	return (point.x - m_x)*(point.x - m_x) +
		   (point.y - m_y)*(point.y - m_y);
}
void WaterParticle::updateVelocity() {

}

void WaterParticle::updatePosition() {
	m_x += m_x_velocity;
	m_y += m_y_velocity;
}

void WaterParticle::checkColisions(std::vector <sf::Vector2f> points) {
	
	for (auto &i : points) {
		if (squareDistance(i) < pow(m_r, 2)) {
			m_x_velocity = 0;
			m_y_velocity = 0;
		}
	}

}
