#include "Workspace.hpp"


void Workspace::draw(sf::RenderTarget& target, sf::RenderStates states)const {

	if (bezier->isControlPoint) { //Bez jaj funkcja const a zmieniamy skladowa bezier ?!
		bezier->CalcQuadBezier();
	}
	
	target.draw(sprite);
	for (auto &line : lines)
	{
		target.draw(line);
	}



//	FillSpaceBetweenPoints(target, states);		todo
	//DrawLinesGroup(target, states);				// rysuje utworzone linie
	//if
	if (bezier->isControlPoint) {
		bezier->CalcQuadBezier();
		DrawCurrentCurve(target, states);
	}
	else
		DrawCurrentLine(target, states);	// rysuje linie tymczasowa, podglad gdzie sie pojawi jak nacisniemy LPM
//	DrawDotsGroup(target, states);				// puste, bedzie mozna potem zrobic jakies kropki czy co tam chcemy
	
}
void Workspace::Update() //Uaktualnia punkty i teksture do rysowania
{
	count = 0;
	visiblePoints.clear();
	visiblePoints.push_back({ 0.0f, static_cast<float>(size->y) });
	sf::FloatRect rect({ 0.0f, 0.0f }, { static_cast<float>(size->x), static_cast<float>(size->y) });
	for (auto &segment : data->segments)
	{
		if (rect.intersects(segment.rect))
		{
			count += segment.points.size();
			visiblePoints.insert(visiblePoints.end(), segment.points.begin(), segment.points.end());
		}
	}
	if (CheckAllColisions(visiblePoints.back(), { visiblePoints.back().x, static_cast<float>(size->y) }))
	{
		count++;
		lines.clear();
		for (int i = 0; i < count - 1; ++i)
		{
			if (i < lastGoodCount)
			{
				lines.push_back(sfLine(visiblePoints[i], visiblePoints[i + 1], sf::Color::Black, 4.0));
			}
			else
			{
				lines.push_back(sfLine(visiblePoints[i], visiblePoints[i + 1], sf::Color::Yellow, 4.0));
			}
		}
		return;
	}
	visiblePoints.push_back({ visiblePoints.back().x, static_cast<float>(size->y) });
	visiblePoints.push_back({ 0.0f, static_cast<float>(size->y) });
	count += 3; 
	lastGoodCount = count - 2;
	sf::RenderTexture renderTexture;
	renderTexture.create(size->x, size->y);
	renderTexture.clear(sf::Color::White);

	lines.clear();
	for (int i = 0; i < count - 1; ++i)
	{
		lines.push_back(sfLine(visiblePoints[i], visiblePoints[i + 1], sf::Color::Black, 4.0));
		renderTexture.draw(sfLine(visiblePoints[i], visiblePoints[i + 1], sf::Color::Black, 3.0));
	}
	renderTexture.display();
	sf::Image tmpImage = renderTexture.getTexture().copyToImage();
	sf::Image *imagePtr = &tmpImage;

	for (int i = 0; i < size->x * size->y; ++i)
	{
		boolPixels[i] = false;
	}

	if (count > 3) {
		struct Points
		{
			bool value;
			int x;
			int y;
		};
		Points neighbours[8];
		Points *points = new Points[size->x * size->y];
		boolPixels[(size->y - 4) * size->x + 4] = true;
		points[0] = { true, 4, static_cast<int>(size->y - 4) };
		int i = 0, k = 0;
		Points point, neighbour;
		while (i >= 0)
		{
			point = points[i];
			--i;
			k = 0;
			for (int _X = -1; _X <= 1; ++_X)
			{
				for (int _Y = -1; _Y <= 1; ++_Y)
				{
					if (_Y || _X)
					{
						neighbours[k] = { boolPixels[(point.y + _Y) * size->x + point.x + _X], point.x + _X, point.y + _Y };
						++k;
					}
				}
			}
			k = 8;
			while(k)
			{
				neighbour = neighbours[k];
				if (!neighbour.value && abs(imagePtr->getPixel(point.x, point.y).r - imagePtr->getPixel(neighbour.x, neighbour.y).r) < 128)
				{
					boolPixels[(neighbour.y) * size->x + neighbour.x] = true;
					++i;
					points[i] = neighbour;
				}
				--k;
			}
		}
		delete[] points;
	}
	texture.create(size->x, size->y);

	sf::Uint8 *pixels = new sf::Uint8[size->x * size->y * 4];
	for (int x = 0; x < size->x; ++x)
	{
		for (int y = 0; y < size->y; ++y)
		{
			if (boolPixels[y * size->x + x])
			{
				pixels[4 * (y * size->x + x) + 0] = 124;
				pixels[4 * (y * size->x + x) + 1] = 44;
				pixels[4 * (y * size->x + x) + 2] = 36;
				pixels[4 * (y * size->x + x) + 3] = 255;
			}
			else
			{
				pixels[4 * (y * size->x + x) + 0] = 200;
				pixels[4 * (y * size->x + x) + 1] = 247;
				pixels[4 * (y * size->x + x) + 2] = 255;
				pixels[4 * (y * size->x + x) + 3] = 255;
			}
		}
	}
	texture.update(pixels);
	sprite.setTexture(texture);
	delete[] pixels;
}

Workspace::Workspace(Data *newData, sf::Vector2u *newSize, sf::Vector2f *newPosition) : data(newData), size(newSize), position(newPosition) {
	boolPixels = new bool[size->x * size->y];
	bezier = new Bezier();
	originalSize = *size;
	startingPoint = { 0.0, static_cast<float>(size->y / 2.0) };		// punkt poczatkowy rysowania, potem sie zrobi zeby uzytkownik mogl go wybrac
	endingPoint = mousePosition = startingPoint;					// ostatni punkt rysowania, na razie to jest ten sam
	mainPoints.push_back(startingPoint);							
	mainPoints.push_back(startingPoint);
}

Workspace::~Workspace()
{
	delete bezier;
	delete[] boolPixels;
}

bool Workspace::isMouseInWorkspaceArea(double x, double y) {		// sprawdza czy myszka jest w workspace, skaluje sie, bo size trzyma wskaznik na aktualny rozmiar

	if (position->x <= x && x <= position->x + size->x) 
		if (position->y <= y && y <= position->y + size->y) 
			return true;
	return false;
}

void Workspace::Resize() {
	//todo
}


void Workspace::DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const {

	for (int i = 0; i < mainPoints.size() - 1; i++) {
		sfLine currentLine(mainPoints[i], mainPoints[i + 1], sf::Color::Black, 1.0);	// wlasna klasa sfLine, wzialem ja z forum SFML, podaje sie punkty i grubosc lini do narysowania
		currentLine.draw(target, states);		// rysuja ta linie
	}
}


void Workspace::FillSpaceBetweenPoints(sf::RenderTarget& target, sf::RenderStates states) const {	// todo

	
}

void Workspace::DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const {	// rysuje utworzone linie

	sf::Color lineColor;
	if (CheckAllColisions(mainPoints.back(), mousePosition)) {
		lineColor = sf::Color::Red;
	}
	else {
		lineColor = sf::Color::Green;
	}
	sfLine currentLine(mainPoints.back(), mousePosition, lineColor, 3.0);
	currentLine.draw(target, states);
}

bool Workspace::CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const {	// sprawdza kolizje lini z wszystkimi pozostalymi liniami

	for (int i = 1; i < mainPoints.size() - 1; i++)	{
		if (CheckColision(mainPoints[i], mainPoints[i + 1], a, b)) {
			return true;
		}
	}
	return false;
}

void Workspace::DrawCurrentCurve(sf::RenderTarget& target, sf::RenderStates states) const {

	sf::Color lineColor;

	if (CheckBezierColisions()) {
		lineColor = sf::Color::Red;
	}
	else {
		lineColor = sf::Color::Green;
	}
	for (int i = 0; i < bezier->points.size() - 1; i++) {
		sfLine currentLine(bezier->points[i], bezier->points[i + 1], lineColor, 3.0);
		currentLine.draw(target, states);
	}
}

bool Workspace::CheckBezierColisions()const{
	
	for (int i = 1; i < bezier->points.size() - 2; i++) {
		for (int j = mainPoints.size() - 1; j >  0; j--) {
			if (CheckColision(mainPoints[j], mainPoints[j - 1], bezier->points[i], bezier->points[i+1])) {
				std::cout << "Bezier colision detected at " << mainPoints[j].x << " " << mainPoints[j].y << " " << 
					mainPoints[j - 1].x << " " << mainPoints[j - 1].y << std::endl;
				return true;
			}
		}
	}
	return false;
}

inline bool Workspace::CheckColision(sf::Vector2f a1, sf::Vector2f b1, sf::Vector2f a2, sf::Vector2f b2) const {	
	
	// sprawdza kolizje 2 lini:
// a1-------b1 oraz a2---------b2

	int x1 = a1.x;
	int y1 = a1.y;

	int x2 = b1.x;
	int y2 = b1.y;

	int x3 = a2.x;
	int y3 = a2.y;

	int x4 = b2.x;
	int y4 = b2.y;

	float denominator = ((x2 - x1) * (y4 - y3)) - ((y2 - y1) * (x4 - x3));
	float numerator1 = ((y1 - y3) * (x4 - x3)) - ((x1 - x3) * (y4 - y3));
	float numerator2 = ((y1 - y3) * (x2 - x1)) - ((x1 - x3) * (y2 - y1));

	if (denominator == 0) {
		//std::cout << "Denominator equals 0" << std::endl;
		return false;
	}

	//if (denominator == 0) return (numerator1 == 0 && numerator2 == 0);	// sprawdza czy mianownik jest 0

	float r = numerator1 / denominator;
	float s = numerator2 / denominator;
	if ((r >= 0 && r < 1) && (s >= 0 && s <= 1)) {
		/*std::cout << "Colision detected between points : " << x1 << " " << y1 << " " << x2 << " " << y2 << " "
			<< x3 << " " << y3 << " " << x4 << " " << y4 << std::endl;*/
	}
	return (r >= 0 && r < 1) && (s >= 0 && s <= 1);
}

void Workspace::AddPoint(int x, int y) {	// dodaje punkt jesli mozna (nie ma kolizji)

	if (CheckAllColisions(mainPoints.back(), mousePosition) == false) {
		mainPoints.push_back(sf::Vector2f(1.0*x, 1.0*y));
	}
}

void Workspace::UpdateMousePosition(int x, int y) {			// podobno nie potrzebna, potem sie zrobi refaktoryzacje

	mousePosition.x = static_cast<double>(x);
	mousePosition.y = static_cast<double>(y);
	bezier->setControlPoint(x, y);
}

void Workspace::DrawDotsGroup(sf::RenderTarget& target, sf::RenderStates states) const {
	// moze sie przydac 
}

sf::Vector2f & Workspace::getLastPoint() {
	return mainPoints.back();
}

void Workspace::PushBesierPoints() {
	for (std::vector<sf::Vector2f>::const_iterator a = bezier->points.begin(); a != bezier->points.end(); ++a) {
		mainPoints.push_back(*a);
	}
}


void Workspace::PerlinNoise1D(int nCount, double *noiseSeed, int nOctaves, double *surfacePixels)
{

	double fNoise;
	double fScaleAcc = 0;
	double fScale;
	int nPitch;
	int nSample1;
	int nSample2;
	double fBlend;
	double fSample;

	for (int x = 0; x < nCount; ++x) {
		surfacePixels[x] = 0;
	}

	for (int o = 0; o < nOctaves; ++o) {
		fNoise = 0;
		fScale = (1.0 / pow(2, o));
		nPitch = (int)nCount / pow(2, o);
		if (nPitch == 0) {
			break;
		}
		fScaleAcc += fScale;


		for (int x = 0; x < nCount; ++x)
		{
			nSample1 = ((int)x / nPitch)*nPitch;
			nSample2 = (nSample1 + nPitch) % nCount;

			fBlend = static_cast<double>((x - nSample1) / (double)nPitch);
			fSample = (1.0f - fBlend) * noiseSeed[nSample1] + fBlend * noiseSeed[nSample2];

			fNoise = fSample * fScale;
			surfacePixels[x] += (fNoise);

		}

	}
	for (int x = 0; x < nCount; ++x) {
		surfacePixels[x] /= fScaleAcc;
		            std::cout << surfacePixels[x] << std::endl;

	}

}


void Workspace::perlinNoise()
{
	
	int mapWidth =  size->x;
	int mapHeight = size->y;

	double *noiseSeed = new double[mapWidth];
	double *surfaceVector = new double[mapWidth];;

	sf::Uint8  *mapPixels = new sf::Uint8[4*mapWidth*mapHeight];

	for (int i = 0; i < mapWidth; i++)
		noiseSeed[i] = ((double)rand() / (double)RAND_MAX);

	PerlinNoise1D(mapWidth, noiseSeed, 12, surfaceVector);

		for (int x = 0; x < mapWidth; ++x)
			for (int y = 0; y < mapHeight; ++y)
			{
				if (y > static_cast<int>((surfaceVector[x])*mapHeight))
				{
					mapPixels[4 * (y * size->x + x) + 0] = 124;
					mapPixels[4 * (y * size->x + x) + 1] = 44;
					mapPixels[4 * (y * size->x + x) + 2] = 36;
					mapPixels[4 * (y * size->x + x) + 3] = 255;
				}
				else
				{
					mapPixels[4 * (y * size->x + x) + 0] = 200;
					mapPixels[4 * (y * size->x + x) + 1] = 247;
					mapPixels[4 * (y * size->x + x) + 2] = 255;
					mapPixels[4 * (y * size->x + x) + 3] = 255;

				}
			}

		texture.create(size->x, size->y);
		texture.update(mapPixels);
		sprite.setTexture(texture);
		delete[] mapPixels;
		delete[] noiseSeed;
		delete[] surfaceVector;
}