#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

const int SizeCell = 32;

class PlayField{
public:
	Sprite fieldsprite;
	enum colors {GRAY, BLACK, CROSS} color;

	void field_position(int x, int y){
		fieldsprite.setPosition(x * 32, y * 32);
	}

	void change_color(int click)
	{
		switch (click)
		{
		case 0:	{
			if (color == 0 || color == 2){
				fieldsprite.setTextureRect(IntRect(64, 0, 32, 32));
				color = BLACK;
			}else{
				fieldsprite.setTextureRect(IntRect(0, 0, 32, 32));
				color = GRAY;
			}
			break;
		}

		case 1:
		{
			if (color == 0 || color == 1){
			fieldsprite.setTextureRect(IntRect(32, 0, 32, 32));
			color = CROSS;
		}else{
			fieldsprite.setTextureRect(IntRect(0, 0, 32, 32));
			color = GRAY;
		}
			break;
		}

		case 2:
			{
				fieldsprite.setTextureRect(IntRect(0, 0, 32, 32));
				color = GRAY;
				break;
			}

		default:
			break;
		}
	}
};

class JC{
public:
	int m, n; // m - heigth, n - width;
	int **VertData, **HorData;


};

int main()
{

	const int width = 10, height = 15;
	int i = 0, j = 0;

	RenderWindow window(sf::VideoMode(width*32, height*32), "Solver JC", Style::Close);
	Texture fieldtexture;
	fieldtexture.loadFromFile("images/textures.jpg");

	PlayField PartOfField[width][height];
	for (i = 0; i < width; i++){
		for (j = 0; j < height; j++){
			PartOfField[i][j].field_position(i, j);
			PartOfField[i][j].fieldsprite.setTexture(fieldtexture);
			PartOfField[i][j].change_color(2);
		}
	}

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)){
			switch (event.type){
				case Event::Closed:
					window.close();
					break;
				case Event::MouseButtonPressed:
				{
					int xpos = Mouse::getPosition(window).x / 32;
					int ypos = Mouse::getPosition(window).y / 32;
					PartOfField[xpos][ypos].change_color(event.mouseButton.button);
					break;
				}
				default:
					break;
			}
		}

		window.clear();

		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++){
				window.draw(PartOfField[i][j].fieldsprite);
			}
		}

		window.display();
	}
	return 0;
}
