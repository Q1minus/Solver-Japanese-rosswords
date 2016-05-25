#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

const int SizeCell = 32;

int parse(std::string s)
{
	for (int i = 0; i < (int)s.length(); i++)
		if (s[i] >= '0' && s[i] <= '9')
			continue;
		else
			return 0;

		return 1;
}

int strToInt(std::string a_str)
{
	if ((int)a_str.length() <= 3 && parse(a_str) == 1)
		return atoi(a_str.c_str());
	else
		std::cout << "ERROR!" << std::endl;

	return 0;
}

int length_arr(int *arr, int m)
{
	for (int i = 0; i < m; i++)
		if (!arr[i])
			return i++;

	return m;
}

void init_arr(int **parr, int m, int n)
{
	for (int i = 0; i < m; i++) // Zanulaem arr
		for (int j = 0; j < n; j++)
			parr[i][j] = 0;
}

int **create_arr(int m, int n)
{
	int **parr = new int *[m];

	for (int i = 0; i < m; i++)
		parr[i] = new int [n];

	init_arr(parr, m, n);

	return parr;
}

void destroy_arr(int **arr, int m)
{
	for (int i = 0; i < m; i++)
		delete [] arr[i];
	delete [] arr;
}

class FieldCreator{
public:
	int m, n, w, h;
	int topM, topN, leftM, leftN;
	int **arr, **topArr, **leftArr;
	std::string nonName;

	Image menuImage;
	Image fieldImage;

	Texture menuTexture;
	Texture fieldTexture;

	Sprite menuSprite;
	Sprite fieldSprite;
	Sprite matrixSprite;

	Font font;
	Text text;

	FieldCreator(int M, int N)
	{
		m = M; n = N; //m = width, n = heigth cells
		w = 5*SizeCell; h = 2*4*SizeCell;
		topM = topN = 0;
		leftM = leftN = 0;

		font.loadFromFile("Font.ttf");
		text.setFont(font);
		text.setCharacterSize(24);
		text.setColor(Color::Black);
		text.setStyle(Text::Bold);

		arr = create_arr(M, N);
		topArr = create_arr(0, 0);
		leftArr = create_arr(0, 0);

		fieldImage.loadFromFile("images/field.jpg");
		fieldTexture.loadFromImage(fieldImage);
		fieldSprite.setTexture(fieldTexture);
		fieldSprite.setTextureRect(IntRect(0, 0, SizeCell, SizeCell));

		matrixSprite.setTexture(fieldTexture);
		matrixSprite.setTextureRect(IntRect(96, 0, SizeCell, SizeCell));

		menuImage.loadFromFile("images/menu.jpg");
		menuTexture.loadFromImage(menuImage);
		menuSprite.setTexture(menuTexture);
		menuSprite.setTextureRect(IntRect(0, 0, w, h));

	}

	void change_color(int click, int x, int y)
	{
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

	int **read_arr(std::string fileName, int &m, int &n)
	{
		int flag = 1;
		std::string matrix;
		std::ifstream fin;
		fin.open(fileName.c_str());

		/*Создаем Matrix строку, находим m и n для дальнейшего
		создания матрицы-результата*/
		if(!fin.is_open()){
			std::cout << "Error! File not found!" << std::endl;
			return 0;
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

	void Press(int button)
	{
		switch (button) {
			/*OPEN*/
			case 0:
			{
				/*Разрушаем старые матрицы, создаем новые
				(для повторного открытия)*/
				destroy_arr(topArr, topM);
				destroy_arr(leftArr, leftM);
				topArr = create_arr(0, 0);
				leftArr = create_arr(0, 0);
				topN = topM = 0;
				leftN = leftM = 0;
				nonName = "";
				std::string fileName = "";

				/*Вводим имя кроссворда, прописываем полный
				путь, создаем матрицу*/
				std::cout << "Crossword name: ";
				std::cin >> fileName;
				std::string topFile = "Data/TopMatrix/" + fileName;
				std::string leftFile = "Data/LeftMatrix/" + fileName;
				nonName = "Data/" + fileName; ;

				/*Проверка на наличие файла*/
				topArr = read_arr(topFile, topM, topN);
				if (!topArr)
					return;
				leftArr = read_arr(leftFile, leftM, leftN);
				if (!topArr)
					return;

				/*Минимизируем матрицы*/
				minimization_arr(topArr, topM, topN);
				minimization_arr(leftArr, leftM, leftN);

				/*Пересоздаем поле*/
				destroy_arr(arr, m);
				m = topM; n = leftM;
				arr = create_arr(m, n);
			}
			break;

			/*SAVE*/
			case 1:
			{
				/*Задаём матрицы данных*/
				destroy_arr(topArr, topM);
				destroy_arr(leftArr, leftM);
				topM = m; topN = n/2 + 1;
				leftM = n; leftN = m/2 + 1;
				topArr = create_arr(topM, topN);
				leftArr = create_arr(leftM, leftN);

				/*Задаем имя файла, затем полный путь к матрицам*/
				std::cout << "Crossword name: ";
				std::cin >> nonName;
				std::string topFile = "Data/TopMatrix/" + nonName;
				std::string leftFile = "Data/LeftMatrix/" + nonName;
				std::string fieldFile = "Data/" + nonName;
				nonName = fieldFile;

				/*Матрица поля*/
				std::ofstream fieldMatrix;
				fieldMatrix.open(fieldFile.c_str());

				for (int i = 0; i < m; i++) {

					for (int j = 0; j < n; j++) {
						if (arr[i][j] == 2)
							fieldMatrix << arr[i][j] << " ";
						else
							fieldMatrix << 1 << " ";
					}

					fieldMatrix << std::endl;
				}

				fieldMatrix.close();

				/*Верхяя матрица*/
				std::ofstream topMatr;
				topMatr.open(topFile.c_str());

				init_arr(topArr, m, n/2+1);

				/*Заполняем верхнюю матрицу*/
				for (int i = 0; i < m; i++) {
					int k = 0;
					for (int j = 0; j < n; j++) {
						if (arr[i][j] == 2)
							topArr[i][k]++;
						else if (arr[i][j-1] == 2)
							k++;
					}
				}

				minimization_arr(topArr, topM, topN);

				/*Выводим в файл*/
				for (int i = 0; i < m; i++){
					for (int j = 0; j < n/2 + 1; j++)
						topMatr << topArr[i][j] << " ";
					topMatr << std::endl;
				}

				topMatr.close();

				/*Левая матрица*/
				std::ofstream leftMatr;
				leftMatr.open(leftFile.c_str());

				init_arr(leftArr, n, m/2+1);

				/*Заполняем левую матрицу*/
				for (int i = 0; i < n; i++) {
					int q = 0;
					for (int j = 0; j < m; j++) {
						if(arr[j][i] == 2)
							leftArr[i][q]++;
						else if (j > 0 && arr[j-1][i] == 2)
								q++;
					}
				}

				minimization_arr(leftArr, leftM, leftN);

				/*Выводим в файл*/
				for (int i = 0; i < n; i++){
					for (int j = 0; j < m/2 + 1; j++)
						leftMatr << leftArr[i][j] << " ";
					leftMatr << std::endl;
				}

				leftMatr.close();
			}
			break;

			/*CLEAR*/
			case 2:
				for (int i = 0; i < m; i++)
					for (int j = 0; j < n; j++)
						arr[i][j] = 0;
				break;

			/*SOLVE*/
			case 3:
			{
				arr = read_arr(nonName, m, n);
			}
				break;
			default:
				break;
		}
	}

	void minimization_arr(int **arr, int &m, int &n)
	{
		/*Пробегаемся по массиву, пока не встретим
		ряд нулей, удаляем все ряды нулей*/
		for (int i = 0; i < n; i++) {
			int flag = 0;

			for (int j = 0; j < m; j++) {
				/*Если встретим нулевой столбец, удаляем
				его и всё, что после*/
				if (arr[j][i] != 0){
					flag = 0;
					break;
				}
				else
					flag = 1;
			}

			if (flag == 1){
				n = i;
				break;
			}
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

	}
	else {
		std::cout << "Set min size 8x8\n" << std::endl;
		m = 8;
		n = 8;
	}

	/*Создаем поле*/
	FieldCreator creator(m, n);

	RenderWindow window(sf::VideoMode((creator.m + 5)*SizeCell, creator.n*SizeCell), "Creator JC", sf::Style::Close);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event)){
			if(event.type == sf::Event::Closed)
				window.close();

      /* "Оживляем" меню*/
			if(event.type == Event::MouseButtonPressed){
				int xpos = Mouse::getPosition(window).x / SizeCell;
				int ypos = Mouse::getPosition(window).y / SizeCell;

				/*Меняем цвет клеток по клику*/
				if (xpos >= 5 + creator.leftN && ypos >= creator.topN){
					creator.change_color(event.mouseButton.button, xpos - 5 - creator.leftN, ypos - creator.topN);
				}
				else if (xpos < 5){ //menu item
					if(ypos >= 0 && ypos < 2){//OPEN
						creator.Press(0);
						int X = (creator.leftN + creator.m + 5)*SizeCell;
						int Y = (creator.n + creator.topN)*SizeCell;
						window.create(sf::VideoMode(X, Y), "JCrossword", sf::Style::Close);

					}
					if (ypos > 1 && ypos < 4){//LOAD
						creator.Press(1);
						int X = (creator.leftN + creator.m + 5)*SizeCell;
						int Y = (creator.n + creator.topN)*SizeCell;
						window.create(sf::VideoMode(X, Y), "JCrossword", sf::Style::Close);

					}
					if (ypos > 3 && ypos < 6)//CLEAR
						creator.Press(2);
					if (ypos > 5 && ypos < 8)//SOLVE
						creator.Press(3);

				}

			}
		}

		window.clear();

		/*Рисуем верхнюю матрицу*/
		for (int i = 0; i < creator.topM; i++) {
			for (int j = 0; j < creator.topN; j++) {
				int X = (i + 5 + creator.leftN) * SizeCell;
				int Y = j * SizeCell;
				creator.matrixSprite.setPosition(X, Y);
				window.draw(creator.matrixSprite);

			}
		}

		for (int i = 0; i < creator.topM; i++) {
			int len = length_arr(creator.topArr[i], creator.topM);

			for (int j = 0; j < creator.topN; j++) {
				int X = (i + 5 + creator.leftN) * SizeCell;
				int Y = (j + creator.topN - len) * SizeCell;

				if (creator.topArr[i][j]){
					std::ostringstream matrixItem;
					matrixItem << creator.topArr[i][j];
					creator.text.setPosition(X + 2, Y + 2);
					creator.text.setString(matrixItem.str());
					window.draw(creator.text);
				}

			}
		}
		/*Рисуем левую матрицу*/
		for (int i = 0; i < creator.leftM; i++) {
			for (int j = 0; j < creator.leftN; j++) {
				int X = (j + 5) * SizeCell;
				int Y =  (i + creator.topN) * SizeCell;
				creator.matrixSprite.setPosition(X, Y);
				window.draw(creator.matrixSprite);
			}
		}

		for (int i = 0; i < creator.leftM; i++) {
			int len = length_arr(creator.leftArr[i], creator.leftN);

			for (int j = 0; j < creator.leftN; j++) {
				int X = (j + 5 + creator.leftN - len) * SizeCell;
				int Y =  (i + creator.topN) * SizeCell;

				if (creator.leftArr[i][j]){
					std::ostringstream matrixItem;
					matrixItem << creator.leftArr[i][j];
					creator.text.setPosition(X + 2, Y + 2);
					creator.text.setString(matrixItem.str());
					window.draw(creator.text);
				}
			}
		}

		/*Рисуем поле с привязкой к массиву*/
		for (int j = 0; j < creator.n; j++){
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

				creator.fieldSprite.setPosition( (creator.leftN + i + 5)*SizeCell, (creator.topN + j) * SizeCell);
				window.draw(creator.fieldSprite);
				//std::cout << creator.arr[i][j] << " ";
			}
			//std::cout  << std::endl;
		}

		/*Рисуем меню*/
		window.draw(creator.menuSprite);
		window.display();
	}
	return 0;
}
