
#include "Workspace.hpp"

void Workspace::draw(sf::RenderTarget& target, sf::RenderStates states)const {

	target.draw(*sprite);
	/*for (auto &line : lines)
	{
		target.draw(line);
	}*/


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

void Workspace::Update(bool waterFlag) //Uaktualnia punkty i teksture do rysowania
{

	ans = std::async([&]() mutable
	{
		if (async) return;
		async = true;

		area.left = 0;// moveVector.x;
		area.top = 0;// moveVector.y;
		count = 0;
		visiblePoints.clear();
		sf::FloatRect rect(area);
		std::list<sf::CircleShape> circlePoints;
		int areaWidth = static_cast<int>(area.width);
		int areaHeight = static_cast<int>(area.height);
		sf::Vector2f offset = { area.left, area.top }, canvasOffset;
		bool first = true;
		float minX, minY, maxX, maxY;
		for (auto &segment : data->segments)
		{
			if (rect.intersects(segment.rect))
			{
				count += segment.points.size();
				visiblePoints.insert(visiblePoints.end(), segment.points.begin(), segment.points.end());
				if (first)
				{
					minX = segment.rect.left;
					minY = segment.rect.top + segment.rect.height;
					maxX = segment.rect.left + segment.rect.width;
					maxY = segment.rect.top;
					first = false;
				}
				else
				{
					if (segment.rect.left < minX)
					{
						minX = segment.rect.left;
					}
					if (segment.rect.left + segment.rect.width > maxX)
					{
						maxX = segment.rect.left + segment.rect.width;
					}
					if (segment.rect.top + segment.rect.height > minY)
					{
						minY = segment.rect.top + segment.rect.height;
					}
					if (segment.rect.top < maxY)
					{
						maxY = segment.rect.top;
					}
				}
			}
		}

		if (count == 0)
		{
			doubleBuffor->setTexture({});
			doubleBuffor->setPosition(-areaOffset);
			spriteRelativePosition = -areaOffset;
			sf::Sprite *ptr;
			ptr = doubleBuffor;
			doubleBuffor = sprite;
			sprite = ptr;
			async = false;
			return;
		}
		minY += 100;
		visiblePoints.insert(visiblePoints.begin(), { minX, visiblePoints[0].y });
		visiblePoints.insert(visiblePoints.begin(), { minX, minY });


		if (visiblePoints.back().x - minX < 20)
		{
			visiblePoints.push_back({ minX + 20, minY });
		}
		else
		{
			visiblePoints.push_back({ visiblePoints.back().x, minY });
		}
		visiblePoints.push_back({ minX, minY });
		count += 4;
		lastGoodCount = count - 3;

		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		//settings.antialiasingLevel = config(L"antialiasing"); TODO
		int canvasWidth = static_cast<int>(std::max(std::max(maxX - minX + 1, area.width), area.width + area.left - minX + 1));
		int canvasHeight = static_cast<int>(std::max(std::max(minY - maxY + 1, area.height), area.height + area.top - maxY + 1));
		canvasOffset.x = std::max(area.left - minX, 0.0f);
		canvasOffset.y = std::max(area.top - maxY, 0.0f);
		sf::RenderTexture renderTexture;
		renderTexture.create(canvasWidth, canvasHeight, settings);
		renderTexture.clear(sf::Color::White);

		lines.clear();
		for (int i = 0; i < count - 1; ++i)
		{
			if (isPoints)
			{
				circlePoints.push_back(sf::CircleShape(2.0f));
				circlePoints.back().setPosition(visiblePoints[i] - offset - sf::Vector2f(1.0f, 1.0f));
				circlePoints.back().setFillColor(sf::Color::Red);
			}
			lines.push_back(sfLine(visiblePoints[i] - offset, visiblePoints[i + 1] - offset, sf::Color::Black, 4.0));
			renderTexture.draw(sfLine(visiblePoints[i] - offset + canvasOffset, visiblePoints[i + 1] - offset + canvasOffset, sf::Color::Black, 3.0));
		}
		if (isPoints)
		{
			circlePoints.push_back(sf::CircleShape(2.0f));
			circlePoints.back().setPosition(visiblePoints[count - 1] - offset - sf::Vector2f(1.0f, 1.0f));
			circlePoints.back().setFillColor(sf::Color::Red);
		}
		renderTexture.display();
		sf::Image tmpImage = renderTexture.getTexture().copyToImage();
		sf::Image *imagePtr = &tmpImage;
		/////
		boolPixels = new bool[canvasWidth * canvasHeight];
		for (int i = 0; i < canvasWidth * canvasHeight; ++i)
		{
			boolPixels[i] = false;
		}
		sf::CircleShape circl(4);
		if (count > 3) {
			struct Points
			{
				bool value;
				int x;
				int y;
			};
			Points neighbours[8];
			Points *points = new Points[canvasWidth * canvasHeight];
			boolPixels[(static_cast<int>(visiblePoints[0].y - offset.y + canvasOffset.y) - 8) * canvasWidth + static_cast<int>(visiblePoints[0].x - offset.x + canvasOffset.x) + 8] = true;
			points[0] = { true, static_cast<int>(visiblePoints[0].x - offset.x + canvasOffset.x) + 8, static_cast<int>(visiblePoints[0].y - offset.y + canvasOffset.y) - 8 };
			circl.setFillColor(sf::Color::Red);
			circl.setPosition({ visiblePoints[0].x - offset.x + canvasOffset.x, visiblePoints[0].y - offset.y + canvasOffset.y });
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
							if (point.y + _Y >= 0 && point.y + _Y < canvasHeight && point.x + _X >= 0 && point.x + _X < canvasWidth)
							{
								neighbours[k] = { boolPixels[(point.y + _Y) * canvasWidth + point.x + _X], point.x + _X, point.y + _Y };
								++k;
							}
						}
					}
				}
				//k = 8;
				while (k)
				{
					--k;
					neighbour = neighbours[k];
					if (!neighbour.value
						&& !abs(imagePtr->getPixel(point.x, point.y).r - imagePtr->getPixel(neighbour.x, neighbour.y).r))
					{
						boolPixels[(neighbour.y) * canvasWidth + neighbour.x] = true;
						++i;
						points[i] = neighbour;
					}
					//--k;
				}
			}
			delete[] points;

			////////////////////////////////////
			int x, y;
			bool fl = true;

			while (!waterToSpill.empty() || isSimpleWater)
			{
				myLittleWater newWater;
				if (isSimpleWater)
				{
					newWater = { simpleWater.x, simpleWater.y, 1.0f };
				}
				else
				{
					newWater = waterToSpill.back();
					waterToSpill.pop_back();
				}
				while (true)
				{
					x = newWater.x - offset.x + canvasOffset.x;
					y = newWater.y - offset.y + canvasOffset.y;

					sf::Vector2i saveLeftPoint{ x, y }, saveRightPoint{ x, y };
					int deep = y;
					bool rightFlag, leftFlag;
					do
					{
						rightFlag = leftFlag = true;
						saveRightPoint.x = saveLeftPoint.x;
						while (rightFlag)
						{
							if (!boolPixels[(deep + 1)* canvasWidth + saveRightPoint.x] && !data->CheckWater(saveRightPoint.x + offset.x - canvasOffset.x, deep + 1 + offset.y - canvasOffset.y))
							{
								++deep;
								if (deep == canvasHeight - 1)
								{
									waterToSpill.clear();
									delete[] boolPixels;
									async = false;
									return;
								}
							}
							else if (!boolPixels[(deep)* canvasWidth + saveRightPoint.x + 1])
							{
								++saveRightPoint.x;
								if (saveRightPoint.x == canvasWidth - 1)
								{
									waterToSpill.clear();
									delete[] boolPixels;
									async = false;
									return;
								}
							}
							else
							{
								rightFlag = false;
							}
						}
						saveRightPoint.y = deep;
						saveLeftPoint.x = saveRightPoint.x;
						while (leftFlag)
						{
							if (!boolPixels[(deep + 1)* canvasWidth + saveLeftPoint.x] && !data->CheckWater(saveLeftPoint.x + offset.x - canvasOffset.x, deep + 1 + offset.y - canvasOffset.y))
							{
								++deep;
								if (deep == canvasHeight - 1)
								{
									waterToSpill.clear();
									delete[] boolPixels;
									async = false;
									return;
								}
							}
							else if (!boolPixels[(deep)* canvasWidth + saveLeftPoint.x - 1])
							{
								--saveLeftPoint.x;
								if (saveLeftPoint.x == 0)
								{
									waterToSpill.clear();
									delete[] boolPixels;
									async = false;
									return;
								}
							}
							else
							{
								leftFlag = false;
							}
							saveLeftPoint.y = deep;
						}

					} while (saveRightPoint.y != saveLeftPoint.y);
					if (saveLeftPoint.x > 0 && saveRightPoint.x < canvasWidth)
					{
						std::vector<myLine> UP;
						int upLeft = saveLeftPoint.x, upRight = saveLeftPoint.x;
						while (true)
						{
							while (boolPixels[(deep - 1)* canvasWidth + upLeft])
							{
								upLeft++;
								if (upLeft >= saveRightPoint.x)
								{
									break;
								}
							}
							while (true)
							{
								if (!boolPixels[(deep - 1)* canvasWidth + upLeft - 1])
								{
									--upLeft;
								}
								else break;
							}
							upRight = upLeft;
							while (true)
							{
								if (!boolPixels[(deep - 1)* canvasWidth + upRight + 1])
								{
									++upRight;
								}
								else break;
							}
							UP.push_back({ static_cast<int>(upLeft + offset.x - canvasOffset.x), static_cast<int>(upRight + offset.x - canvasOffset.x) });
							if (upRight >= saveRightPoint.x)
							{
								break;
							}
							upLeft = upRight + 1;

						}
						std::vector<myLine> toFill;
						toFill = data->CreateWater(saveLeftPoint.x + offset.x - canvasOffset.x
							, saveRightPoint.x + offset.x - canvasOffset.x
							, deep + offset.y - canvasOffset.y
							, newWater.value, UP);
						for (auto &fill : toFill)
						{
							waterToSpill.push_back({ static_cast<int>(fill.left), static_cast<int>(deep + offset.y - canvasOffset.y), 1.0f });
						}
						bool flagDel = false;
						for (auto &fill : toFill)
						{
							if (!data->CheckWater(fill.left, deep + offset.y - canvasOffset.x))
							{
								flagDel = true;
								break;
							}
						}
						if (flagDel)
						{
							//data->RemoveWater();
						}

					}
					--newWater.value;

					//if(newWater.value < 0 || deep == y + 1)
					if (isSimpleWater && deep == y)
					{
						isSimpleWater = false;
					}
					break;

				}
			}

			////////////////////////////////////
		}
		sf::Texture tmpTexture;
		tmpTexture.create(canvasWidth, canvasHeight);
		sf::Uint8 *pixels = new sf::Uint8[canvasWidth * canvasHeight * 4];
		for (int x = 0; x < canvasWidth; ++x)
		{
			for (int y = 0; y < canvasHeight; ++y)
			{
				if (boolPixels[y * canvasWidth + x])
				{
					pixels[4 * (y * canvasWidth + x) + 0] = 124;
					pixels[4 * (y * canvasWidth + x) + 1] = 44;
					pixels[4 * (y * canvasWidth + x) + 2] = 36;
					pixels[4 * (y * canvasWidth + x) + 3] = 255;
				}
				else
				{
					pixels[4 * (y * canvasWidth + x) + 0] = 200;
					pixels[4 * (y * canvasWidth + x) + 1] = 247;
					pixels[4 * (y * canvasWidth + x) + 2] = 255;
					pixels[4 * (y * canvasWidth + x) + 3] = 255;
				}
			}
		}
		tmpTexture.update(pixels);
		sf::Sprite tpmSprite(tmpTexture);
		tpmSprite.setPosition(-canvasOffset);
		renderTexture.draw(tpmSprite);
		for (auto &line : lines)
		{
			renderTexture.draw(line);
		}
		sf::Vertex waterLine[2];
		waterLine[0].color = sf::Color::Blue;
		waterLine[1].color = sf::Color::Blue;
		for (auto &water : data->water)
		{
			waterLine[0].position = sf::Vector2f(water.left, water.deep) - offset;
			waterLine[1].position = sf::Vector2f(water.right, water.deep) - offset;
			renderTexture.draw(waterLine, 2, sf::Lines);
		}
		//renderTexture.draw(circl);
		for (auto &circle : circlePoints)
		{
			renderTexture.draw(circle);
		}
		renderTexture.display();
		texture = renderTexture.getTexture();
		doubleBuffor->setTexture(texture);
		spriteRelativePosition = -areaOffset - moveVector + sf::Vector2f(area.left, area.top);
		doubleBuffor->setPosition(spriteRelativePosition);
		sf::Sprite *ptr;
		ptr = doubleBuffor;
		doubleBuffor = sprite;
		sprite = ptr;
		delete[] pixels;
		delete[] boolPixels;
		async = false;
	});
}

void Workspace::Move(sf::Vector2i vector)
{
	moveVector.x += vector.x;
	moveVector.y += vector.y;
	spriteRelativePosition.x -= vector.x;
	spriteRelativePosition.y -= vector.y;
	sprite->setPosition(spriteRelativePosition);
	if (abs(moveVector.x - area.left) > areaOffset.x / 2.0f || abs(moveVector.y - area.top) > areaOffset.y / 2.0f)
	{
		Update();

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
		//	std::cout << surfacePixels[x] << std::endl;

	}

}


void Workspace::perlinNoise()
{

	int mapWidth = size->x;
	int mapHeight = size->y;

	double *noiseSeed = new double[mapWidth];
	double *surfaceVector = new double[mapWidth];;

	sf::Uint8  *mapPixels = new sf::Uint8[4 * mapWidth*mapHeight];

	for (int i = 0; i < mapWidth; i++)
		noiseSeed[i] = ((double)rand() / (double)RAND_MAX);

	PerlinNoise1D(mapWidth, noiseSeed, 12, surfaceVector);

	for (int x = 0; x < mapWidth; ++x) {
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
		AddPoint(x, static_cast<int>((surfaceVector[x])*mapHeight));
	}
	texture.create(size->x, size->y);
	texture.update(mapPixels);
	sprite->setTexture(texture);
	delete[] mapPixels;
	delete[] noiseSeed;
	delete[] surfaceVector;
}

void Workspace::AddWater(int x, int y, float value)
{
	x += moveVector.x + areaOffset.x;
	y += moveVector.y + areaOffset.y;
	std::cout << "x: " << x << " y: " << y << std::endl;
	while (value > 0)
	{
		waterToSpill.push_back({ x, y, 1.0f });
		--value;
	}

}

void Workspace::AddSimpleWater(int x, int y)
{
	simpleWater = { x + static_cast<int>(moveVector.x + areaOffset.x) , y + static_cast<int>(moveVector.y + areaOffset.y) };
	isSimpleWater = true;
}

Workspace::Workspace(Data *newData, sf::Vector2u *newSize, sf::Vector2f *newPosition) : data(newData), size(newSize), position(newPosition) {
	areaOffset = { static_cast<float>(size->x) * 0.3f, static_cast<float>(size->y) * 0.3f };
	spriteRelativePosition = areaOffset;
	area = { -areaOffset, {static_cast<float>(size->x) + areaOffset.x * 2.0f, static_cast<float>(size->y) + areaOffset.y * 2.0f} };
	moveVector = { area.left, area.top };
	doubleBuffor = new sf::Sprite;
	sprite = new sf::Sprite;

	bezier = new Bezier();
	originalSize = *size;
	startingPoint = { 0.0, static_cast<float>(size->y / 2.0) };		// punkt poczatkowy rysowania, potem sie zrobi zeby uzytkownik mogl go wybrac
	endingPoint = mousePosition = startingPoint;					// ostatni punkt rysowania, na razie to jest ten sam
	mainPoints.push_back(startingPoint);
	mainPoints.push_back(startingPoint);
}

Workspace::~Workspace()
{
	delete sprite;
	delete doubleBuffor;
	delete bezier;
}

bool Workspace::isMouseInWorkspaceArea(double x, double y) {		// sprawdza czy myszka jest w workspace, skaluje sie, bo size trzyma wskaznik na aktualny rozmiar

	if (position->x <= x && x <= position->x + size->x)
		if (position->y <= y && y <= position->y + size->y)
			return true;
	return false;
}


void Workspace::DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const {

	for (int i = 0; i < mainPoints.size() - 1; i++) {
		sfLine currentLine(mainPoints[i], mainPoints[i + 1], sf::Color::Black, 1.0);	// wlasna klasa sfLine, wzialem ja z forum SFML, podaje sie punkty i grubosc lini do narysowania
		currentLine.draw(target, states);		// rysuja ta linie
	}
}


void Workspace::DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const {	// rysuje utworzone linie

	sf::Color lineColor;
	if (CheckAllColisions(mainPoints.back(), mousePosition + moveVector + areaOffset)) {
		lineColor = sf::Color::Red;
	}
	else {
		lineColor = sf::Color::Green;
	}
	sfLine currentLine(mainPoints.back() - moveVector - areaOffset, mousePosition, lineColor, 3.0);
	currentLine.draw(target, states);
}

bool Workspace::CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const {	// sprawdza kolizje lini z wszystkimi pozostalymi liniami

	for (int i = 1; i < mainPoints.size() - 1; i++) {
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
		sfLine currentLine(bezier->points[i] - moveVector - areaOffset, bezier->points[i + 1] - moveVector - areaOffset, lineColor, 3.0);
		currentLine.draw(target, states);
	}
}

bool Workspace::CheckBezierColisions()const {

	for (int i = 1; i < bezier->points.size() - 2; i++) {
		for (int j = mainPoints.size() - 1; j > 0; j--) {
			if (CheckColision(mainPoints[j], mainPoints[j - 1], bezier->points[i], bezier->points[i + 1])) {
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

bool Workspace::AddPoint(int x, int y) {	// dodaje punkt jesli mozna (nie ma kolizji)

	if (CheckAllColisions(mainPoints.back(), mousePosition + moveVector + areaOffset) == false) {
		mainPoints.push_back(sf::Vector2f(mousePosition + moveVector + areaOffset));
		return true;
	}
	return false;
}

void Workspace::UpdateMousePosition(int x, int y) {			// podobno nie potrzebna, potem sie zrobi refaktoryzacje

	mousePosition.x = static_cast<double>(x);
	mousePosition.y = static_cast<double>(y);
	bezier->setControlPoint(x + moveVector.x + areaOffset.x, y + moveVector.y + areaOffset.y);
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


