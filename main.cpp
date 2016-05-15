/*Field creator*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <malloc.h>
#include <SFML/Graphics.hpp>

using namespace sf;

const int SizeCell = 32;

int parse(std::string s){
	for (int i = 0; i < (int)s.length(); i++)
		if (s[i] >= '0' && s[i] <= '9')
			continue;
		else
			return 0;

		return 1;
}

int strToInt(std::string a_str){
	if ((int)a_str.length() <= 3 && parse(a_str) == 1)
		return atoi(a_str.c_str());
	else
		std::cout << "ERROR!" << std::endl;

	return 0;
}

class FieldCreator{
public:
	int m, n, w, h;
	int topM, topN, leftM, leftN;
	int **arr, **topArr, **leftArr;

	Image menuImage;
	Texture menuTexture;
	Sprite menuSprite;
	Image fieldImage;
	Texture fieldTexture;
	Sprite fieldSprite;

	void init_arr(int **parr, int m, int n){
		for (int i = 0; i < m; i++) // Zanulaem arr
			for (int j = 0; j < n; j++)
				parr[i][j] = 0;
	}

	int **create_arr(int m, int n) {
		int **parr = new int *[m];

		for (int i = 0; i < m; i++)
			parr[i] = new int [n];

		init_arr(parr, m, n);

		return parr;
	}

	void destroy_arr(int **arr, int m) {
    for (int i = 0; i < m; i++)
    	delete [] arr[i];
    delete [] arr;
	}

	FieldCreator(int M, int N){
		m = M; n = N; //m = width, n = heigth cells
		w = 5*SizeCell; h = 2*4*SizeCell;
		topM = topN = 0;
		leftM = leftN = 0;

		arr = create_arr(M, N);
		topArr = create_arr(0, 0);
		leftArr = create_arr(0, 0);

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

	int **read_arr(std::string fileName, int &m, int &n){
		int flag = 1;
		std::string matrix;
		std::ifstream fin;
		fin.open(fileName.c_str());

		/*Создаем Matrix строку, находим m и n для дальнейшего
		создания матрицы-результата*/
		if(!fin.is_open()){
			std::cout << "Error! File not found!" << std::endl;
		}
		else{
			/*Пробегаемся до конца файла*/
			while (!fin.eof()) {
				std::string s;
				getline(fin, s);
				matrix += s + '\n';

				/*Пробегаемся по строке*/
				if(flag == 1){
					for (int i = 0; i < (int)s.length(); i++)
						if(s[i] == ' ')
							n++;
					flag = 0;
				}
				m++;

			}
		}
		fin.close();
		matrix[matrix.length()-1] = '\0';

		/*Matrix - строка, в которой хранится матрица из файла,
		последний элемент ненужный нам \n, мы его удаляем, --m - из-за \n*/

		int **parr = create_arr(--m, n);
		int i = 0, k = 0, kk = 0, j = 0;

		/*Бежим по этой строке*/
		while (matrix[i] != '\0'){
			std::string numb;
			/*Если не пробел, то цифру записываем в вспомогательную
			строку numb, если пробел, то записываем всё из numb в матрицу-результат,
			чистим numb*/
			if(matrix[i] != ' ') {
				numb[j++] = matrix[i];
			}
			else {
				parr[k][kk++] = atoi(numb.c_str());
				j = 0;
				while (numb[j] != '\0')
					numb[j++] = 0;
				j = 0;
			}
			/*Если \n переходим к следующему указателю по m,
			указатель по n возвращаем в начало*/
			if (matrix[i] == '\n'){
				k++;
				kk = 0;
			}
			i++;
		}

		return parr;
	}

	void Press(int button){ // Activated menu
		switch (button) {
			case 0:{//OPEN
				/*Разрушаем старые поля, создаем новые
				(для повторного открытия)*/
				destroy_arr(topArr, topM);
				destroy_arr(leftArr, leftM);
				topArr = create_arr(0, 0);
				leftArr = create_arr(0, 0);
				topN = topM = 0;
				leftN = leftM = 0;
				std::string fileName = "";

				/*Вводим имя кроссворда, прописываем полный
				путь, создаем поле-матрицу*/
				std::cout << "Crossword name: ";
				std::cin >> fileName;
				std::string topFile = "Data/TopMatrix/" + fileName;
				std::string leftFile = "Data/LeftMatrix/" + fileName;
				topArr = read_arr(topFile, topM, topN);
				leftArr = read_arr(leftFile, leftM, leftN);

			}
			break;
			case 1:{//SAVE
				/*Задаём матрицы данных*/
				topM = m; topN = n/2 + 1;
				leftM = n; leftN = m/2+1;
				topArr = create_arr(topM, topN);
				leftArr = create_arr(leftM, leftN);

				/*Задаем имя файла, затем полный путь к матрицам*/
				std::string fileName;
				std::cout << "Crossword name: ";
				std::cin >> fileName;
				std::string topFile = "Data/TopMatrix/" + fileName;
				std::string leftFile = "Data/LeftMatrix/" + fileName;

				/*Верхяя матрица*/
				std::ofstream topMatr;
				topMatr.open(topFile.c_str());

				init_arr(topArr, m, n/2+1);

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

				/*Левая матрица*/
				std::ofstream leftMatr;
				leftMatr.open(leftFile.c_str());

				init_arr(leftArr, n, m/2+1);

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
			case 2://CLEAR
				for (int i = 0; i < m; i++)
					for (int j = 0; j < n; j++)
						arr[i][j] = 0;
				break;
			case 3://SOLVE
				break;
		}
	}
};

int main()
{
	/*Спрашиваем, хотят ли создать свой нонаграм.
	Да - Задаем параметры поля m и n,
	Нет - Появляется меню и поле 8х8(минисальное)*/
	std::string m_str;
	std::string n_str;
	int m = 0, n = 0;
	std::string answer;

	std::cout << "You want to create your nonagram? (y/n): ";
	std::cin >> answer;

 	if (answer == "y" || answer == "yes") {
			std::cout << "-> Min size 8x8" << std::endl;
			std::cout << "-> Max size 100x100" << std::endl;
			std::cout << "-> Horizontal length m = ";
			std::cin >> m_str;
			m = strToInt(m_str);

			std::cout << "-> Vertical length n = ";
			std::cin >> n_str;
			n = strToInt(n_str);

			if (m < 8 || n < 8){
				std::cout << "-> Set min size 8x8" << std::endl;
				m = 8;
				n = 8;
			}
			if(m > 100 && n > 100){
				std::cout << "-> Set max size 100x100" << std::endl;
				m = 100;
				n = 100;
			}

	} else {
		std::cout << "Set min size 8x8\n" << std::endl;
		m = 8;
		n = 8;
	}

	/*Создаем поле*/
	FieldCreator creator(m, n);

	RenderWindow window(sf::VideoMode((creator.m + 5)*SizeCell, creator.n*SizeCell), "Creator JC", Style::Close);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();

      /*Make active field and menu*/
			if(event.type == Event::MouseButtonPressed){
				int xpos = Mouse::getPosition(window).x / SizeCell;
				int ypos = Mouse::getPosition(window).y / SizeCell;

				if (xpos >= 5){//change color cell
					creator.change_color(event.mouseButton.button, xpos - 5, ypos); //
				}
				else if (xpos < 5){ //menu item
					if(ypos >= 0 && ypos < 2)//OPEN
						creator.Press(0);
					if (ypos > 1 && ypos < 4)//LOAD
						creator.Press(1);
					if (ypos > 3 && ypos < 6)//CLEAR
						creator.Press(2);
					if (ypos > 5 && ypos < 8)//SOLVE
						creator.Press(3);
				}

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

				creator.fieldSprite.setPosition( (i + 5)* SizeCell, j * SizeCell);
				window.draw(creator.fieldSprite);
				//std::cout << creator.arr[i][j] << " ";
			}
			//std::cout  << std::endl;
		}

		window.draw(creator.menuSprite);
		window.display();
	}
	return 0;
}
