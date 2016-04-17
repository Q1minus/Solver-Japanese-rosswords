/*Field creator*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>

using namespace sf;

const int SizeCell = 32;

class FieldCreator{
public:
	int m, n, w, h;
	int **arr, **topArr, **leftArr;

	Image menuImage;
	Texture menuTexture;
	Sprite menuSprite;

	Image fieldImage;
	Texture fieldTexture;
	Sprite fieldSprite;

	void zaminusone(int **parr, int m, int n){
		for (int i = 0; i < m; i++) // Zanulaem arr
			for (int j = 0; j < n; j++)
				parr[i][j] = 0;
	}

	int **create_arr(int m, int n) {
		int **parr = new int *[m];

		for (int i = 0; i < m; i++)
			parr[i] = new int [n];

		for (int i = 0; i < m; i++) // Zanulaem arr
			for (int j = 0; j < n; j++)
				parr[i][j] = 0;

		return parr;
	}

	void destroy_arr(int **arr, int m) {
    for (int i = 0; i < m; i++)
    	delete [] arr[i];
    delete [] arr;
	}

	FieldCreator(int M, int N){
		m = M; n = N; //m = width, n = heigth cells
		w = 5*32; h = 2*3*32;

		arr = create_arr(M, N);
		topArr = create_arr(M, N/2 + 1);
		leftArr = create_arr(N, M/2 + 1);

		fieldImage.loadFromFile("images/field.jpg");
		fieldTexture.loadFromImage(fieldImage);
		fieldSprite.setTexture(fieldTexture);
		fieldSprite.setTextureRect(IntRect(0, 0, SizeCell, SizeCell));

		menuImage.loadFromFile("images/menu.jpg");
		menuTexture.loadFromImage(menuImage);
		menuSprite.setTexture(menuTexture);
		menuSprite.setTextureRect(IntRect(0, 0, w, h));

	}

	void change_color(int click, int x, int y){ //change color cell
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

	void Press(int button){ // Activated menu
		switch (button) {
			case 0://open



				break;
			case 1:{//save
				char fileName[20];
				char topFile[40] = "Data/TopMatrix/";
				char leftFile[40] = "Data/LeftMatrix/";

				std::cout << "Crossword name: ";
				std::cin >> fileName;
				strcat(topFile, fileName);
				strcat(leftFile, fileName);

				std::ofstream topMatr; //create file TOP
				topMatr.open(topFile);

				zaminusone(topArr, m, n/2+1);

				for (int i = 0; i < m; i++) { // fill top Matrix int array
					int k = 0;
					for (int j = 0; j < n; j++) {
						if (arr[i][j] == 2)
							topArr[i][k]++;
						else if (arr[i][j-1] == 2)
							k++;
					}
				}

				for (int i = 0; i < m; i++){ // road to file TOP
					for (int j = 0; j < n/2 + 1; j++)
						topMatr << topArr[i][j] << " ";
					topMatr << std::endl;
				}

				topMatr.close();

				std::ofstream leftMatr; //create file LEFT
				leftMatr.open(leftFile);

				zaminusone(leftArr, n, m/2+1);

				for (int i = 0; i < n; i++) {
					int q = 0;
					for (int j = 0; j < m; j++) {
						if(arr[j][i] == 2)
							leftArr[i][q]++;
						else if (j > 0 && arr[j-1][i] == 2)
								q++;
					}
				}

				for (int i = 0; i < n; i++){ // road to file LEFT
					for (int j = 0; j < m/2 + 1; j++)
						leftMatr << leftArr[i][j] << " ";
					leftMatr << std::endl;
				}

				leftMatr.close();
			}
			break;
			case 2://clear
				for (int i = 0; i < m; i++)
					for (int j = 0; j < n; j++)
						arr[i][j] = 0;
				break;
			default:
				break;
		}
	}

};

int main()
{
	int mc = 0, nc = 0;
	std::cout << "m = ";
	std::cin >> mc;
	std::cout << "n = ";
	std::cin >> nc;

	FieldCreator* creator = new FieldCreator(mc, nc);

	RenderWindow window(sf::VideoMode((creator->m + 5)*SizeCell, creator->n*SizeCell), "Creator JC", Style::Close);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();

			if(event.type == Event::MouseButtonPressed){
				int xpos = Mouse::getPosition(window).x / SizeCell;
				int ypos = Mouse::getPosition(window).y / SizeCell;

				if (xpos >= 5){//chanhe color cell
					creator->change_color(event.mouseButton.button, xpos - 5, ypos);
				}
				else if (xpos < 5){ // menu item
					if(ypos >= 0 && ypos < 2)//OPEN
						creator->Press(0);
					if (ypos > 1 && ypos < 4)//LOAD
						creator->Press(1);
					if (ypos > 3 && ypos < 6)//CLEAR
						creator->Press(2);
				}

			}
		}

		window.clear();

		for (int j = 0; j < creator->n; j++){ // draw a field with reference to the array
			for (int i = 0; i < creator->m; i++){
				if (creator->arr[i][j] == 0){//GRAY
					creator->fieldSprite.setTextureRect(IntRect(0, 0, SizeCell, SizeCell));
				}
				if (creator->arr[i][j] == 1){//CROSS
					creator->fieldSprite.setTextureRect(IntRect(32, 0, SizeCell, SizeCell));
				}
				if (creator->arr[i][j] == 2){//BLACK
					creator->fieldSprite.setTextureRect(IntRect(64, 0, SizeCell, SizeCell));
				}

				creator->fieldSprite.setPosition( (i + 5)* SizeCell, j * SizeCell);
				window.draw(creator->fieldSprite);
				//std::cout << creator->arr[i][j] << " ";
			}
			//std::cout  << std::endl;
		}

		window.draw(creator->menuSprite);
		window.display();
	}
	return 0;
}
