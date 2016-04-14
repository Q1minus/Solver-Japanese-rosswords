/*Field creator*/

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

const int SizeCell = 32;

class FieldCreator{
public:
	int m, n;
	int **arr;
	String File;
	enum colors {GRAY, CROSS, BLACK} color;
	Image fieldImage;
	Texture fieldTexture;
	Sprite fieldSprite;

	int **create_arr(int m, int n) {
		int **parr = new int *[m];
		for (int i = 0; i < m; i++){
			parr[i] = new int [n];
		}

		return parr;
	}

	void destroy_arr(int **arr, int m) {
    for (int i = 0; i < m; i++)
    	delete [] arr[i];
    delete [] arr;
	}

	FieldCreator(String F, int M, int N){
		File = F; color = GRAY;
		m = M; n = N;
		fieldImage.loadFromFile("images/" + File);
		fieldTexture.loadFromImage(fieldImage);
		fieldSprite.setTexture(fieldTexture);
		fieldSprite.setTextureRect(IntRect(0, 0, SizeCell, SizeCell));
		arr = create_arr(M, N);

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				arr[i][j] = 0;
	}

	void change_color(int click, int x, int y){
		switch (click) {
			case 0://left button
				if (arr[x][y] == 0 || arr[x][y] == 1){
					arr[x][y] = 2;
				}else{
					arr[x][y] = 0;
				}
				break;
			case 1://right button
				if (arr[x][y] == 0 || arr[x][y] == 2){
					arr[x][y] = 1;
				}else {
					arr[x][y] = 0;
				}
				break;
			case 2:
				arr[x][y] = 0;
				break;
			default:
				break;
		}
	}


};

int main()
{
	FieldCreator creator("field.jpg", 20, 10);

	RenderWindow window(sf::VideoMode(creator.m*32, creator.n*32), "Creator JC", Style::Close);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();

			if(event.type == Event::MouseButtonPressed){
				int xpos = Mouse::getPosition(window).x / SizeCell;
				int ypos = Mouse::getPosition(window).y / SizeCell;
				creator.change_color(event.mouseButton.button, xpos, ypos);
			}

		}

		window.clear();

		for (int j = 0; j < creator.n; j++){ // draw a field with reference to the array
			for (int i = 0; i < creator.m; i++){
				if (creator.arr[i][j] == 0){//GRAY
					creator.fieldSprite.setTextureRect(IntRect(0, 0, SizeCell, SizeCell));
				}
				if (creator.arr[i][j] == 1){//CROSS
					creator.fieldSprite.setTextureRect(IntRect(32, 0, SizeCell, SizeCell));
				}
				if (creator.arr[i][j] == 2){//BLACK
					creator.fieldSprite.setTextureRect(IntRect(64, 0, SizeCell, SizeCell));
				}
				creator.fieldSprite.setPosition(i*SizeCell, j*SizeCell);
				window.draw(creator.fieldSprite);
				//std::cout << creator.arr[i][j] << " ";

			}
			//std::cout  << std::endl;

		}

		window.display();
	}
	return 0;
}
