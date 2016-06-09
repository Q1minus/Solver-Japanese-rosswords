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

class FieldCreator {
private:
	int w, h;
	int topM, topN, leftM, leftN;
	int **arr, **topArr, **leftArr;
	std::string nonName;

	enum States {MAIN, CREATE, SOLVE, CREATOR} state;

	Image fieldImage;
	Texture fieldTexture;
	Sprite fieldSprite;
	Sprite matrixSprite;

	Font font;
	Text text;

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

	void Press(int button)
	{
		switch (button) {
			/*OPENcr*/
			case 0:
			{
				/*Разрушаем старые матрицы, создаем новые
				(для повторного открытия)*/
				destroy_arr(topArr, topM);
				destroy_arr(leftArr, leftM);
				//topArr = create_arr(0, 0);
				//leftArr = create_arr(0, 0);
				topN = topM = 0;
				leftN = leftM = 0;
				nonName = "";

				std::ifstream list;
				list.open("list.txt");
				int i = 0;

				if(!list.is_open()){
					std::cout << "Error! File not open!" << std::endl;
					return;
				}
				else{
					while (!list.eof()) {
						std::string s;
						getline(list, s);
						i++;
					}
				}
				list.close();

				std::string fileName = winList(--i);

				/*Вводим имя кроссворда, прописываем полный
				путь, создаем матрицу*/
				std::string topFile = "Data/TopMatrix/" + fileName;
				std::string leftFile = "Data/LeftMatrix/" + fileName;
				nonName = "Data/" + fileName;

				/*Проверка на наличие файла*/
				topArr = read_arr(topFile, topM, topN);

				leftArr = read_arr(leftFile, leftM, leftN);

				/*Минимизируем матрицы*/
				minimization_arr(topArr, topM, topN);
				minimization_arr(leftArr, leftM, leftN);

				/*Пересоздаем поле*/
				destroy_arr(arr, m);
				//m = topM; n = leftM;

				m = 0; n = 0;
				arr = read_arr(nonName, m, n);
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
				nonName = "";

				/*Задаем имя файла, затем полный путь к матрицам*/
				std::ifstream list;
				list.open("list.txt");
				int i = 0;

				if(!list.is_open()){
					std::cout << "Error! File not open!" << std::endl;
					return;
				}
				else{
					/*Пробегаемся до конца файла*/
					while (!list.eof()) {
						std::string s;
						getline(list, s);
						i++;
					}
				}
				list.close();

				std::stringstream buff;
				buff << "Nonagram#" << i;
				buff >> nonName;

				std::ofstream list2;
				list2.open("list.txt", std::ios::app);

				if(!list2.is_open()){
					std::cout << "Error! File not open!" << std::endl;
					return;
				}
				else
				list2 << nonName << "\n";

				list2.close();

				std::cout << nonName << std::endl;
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

			case 4:
			{
				/*Разрушаем старые матрицы, создаем новые
				(для повторного открытия)*/
				destroy_arr(topArr, topM);
				destroy_arr(leftArr, leftM);
				//topArr = create_arr(0, 0);
				//leftArr = create_arr(0, 0);
				topN = topM = 0;
				leftN = leftM = 0;
				nonName = "";

				std::ifstream list;
				list.open("list.txt");
				int i = 0;

				if(!list.is_open()){
					std::cout << "Error! File not open!" << std::endl;
					return;
				}
				else{
					while (!list.eof()) {
						std::string s;
						getline(list, s);
						i++;
					}
				}
				list.close();

				std::string fileName = winList(--i);

				/*Вводим имя кроссворда, прописываем полный
				путь, создаем матрицу*/
				std::string topFile = "Data/TopMatrix/" + fileName;
				std::string leftFile = "Data/LeftMatrix/" + fileName;
				nonName = "Data/" + fileName;

				/*Проверка на наличие файла*/
				topArr = read_arr(topFile, topM, topN);

				leftArr = read_arr(leftFile, leftM, leftN);

				/*Минимизируем матрицы*/
				minimization_arr(topArr, topM, topN);
				minimization_arr(leftArr, leftM, leftN);

				/*Пересоздаем поле*/
				destroy_arr(arr, m);
				m = topM; n = leftM;

				arr = create_arr(m, n);
			}
				break;

			case 5:
			{
				//int **trueArr = create_arr(m, n);
				int m = 0, n = 0;
				int **trueArr =	read_arr(nonName, m, n);

				for (int i = 0; i < m; i++)
					for (int j = 0; j < n; j++){

						if (trueArr[i][j] == 1 && arr[i][j] == 2)
							arr[i][j] = 3;

						if (trueArr[i][j] == 2 && arr[i][j] == 1)
							arr[i][j] = 3;

					}

					destroy_arr(trueArr, m);
				break;
			}
				break;

			default:
				break;
		}
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


	std::string winList(int i)
	{
		RenderWindow win(sf::VideoMode(5 * SizeCell, i * SizeCell), "List", sf::Style::Close);
		win.setPosition(Vector2i(0, 0));
		std::string result = "";
		std::stringstream buff;

		while (win.isOpen())
		{
			Event ev;

			while (win.pollEvent(ev)) {
				if(ev.type == sf::Event::Closed){
					win.close();
				}

				if(ev.type == Event::MouseButtonPressed){
					int xp = Mouse::getPosition(win).x / SizeCell;
					int yp = Mouse::getPosition(win).y / SizeCell;
					buff << "Nonagram#" << GetName(xp, yp);
					buff >> result;
					win.close();
				}

			}//while event

			win.clear();
			DrawList(&win, i);
			win.display();
		}

		return result;
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

	int GetName(int x, int y)
	{
		return y % 32 + 1;
	}

public:
	int m, n;

	void SetMandN(int M, int N)
	{
		m = M; n = N; //m = width, n = heigth cells
		arr = create_arr(M, N);
	}

	FieldCreator(int M, int N)
	{
		m = M; n = N; //m = width, n = heigth cells
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

	}

	void DrawList(RenderWindow *window, int n)
	{
		text.setCharacterSize(12);
		text.setColor(Color::White);

		int x = 0, y = 0;

		for (int i = 0; i < n; i++)
		{
			std::string nonName;
			std::stringstream buff;
			buff << "Nonagram#" << i+1;
			buff >> nonName;

			text.setString(nonName);
			text.setPosition( (x + 0.5) * SizeCell, (y + 0.5) * SizeCell);
			y += 1;

			window->draw(text);
		}

	}

	void DrawField(RenderWindow *window)
	{
		/*Рисуем поле с привязкой к массиву*/
		for (int j = 0; j < n; j++){
			for (int i = 0; i < m; i++){
				if (arr[i][j] == 0){//GRAY
					fieldSprite.setTextureRect(IntRect(0, 0, SizeCell, SizeCell));
				}
				if (arr[i][j] == 1){//CROSS
					fieldSprite.setTextureRect(IntRect(32, 0, SizeCell, SizeCell));
				}
				if (arr[i][j] == 2){//BLACK
					fieldSprite.setTextureRect(IntRect(64, 0, SizeCell, SizeCell));
				}
				if (arr[i][j] == 3){ //RED
					fieldSprite.setTextureRect(IntRect(128, 0, SizeCell, SizeCell));
				}

				fieldSprite.setPosition( (leftN + i + 5)*SizeCell, (topN + j) * SizeCell);
				window->draw(fieldSprite);
				//std::cout << arr[i][j] << " ";
			}
			//std::cout  << std::endl;
		}

	}

	void DrawLeftMatrix(RenderWindow *window)
	{
		/*Рисуем левую матрицу*/
		for (int i = 0; i < leftM; i++) {
			for (int j = 0; j < leftN; j++) {
				int X = (j + 5) * SizeCell;
				int Y =  (i + topN) * SizeCell;

			//std::cout << leftArr[i][j] << " ";

				matrixSprite.setPosition(X, Y);
				window->draw(matrixSprite);
			}
			//std::cout << std::endl;
		}

		for (int i = 0; i < leftM; i++) {
			int len = length_arr(leftArr[i], leftN);

			for (int j = 0; j < leftN; j++) {
				int X = (j + 5 + leftN - len) * SizeCell;
				int Y =  (i + topN) * SizeCell;

				if (leftArr[i][j]) {
					text.setColor(Color::Black);
					std::ostringstream matrixItem;
					matrixItem << leftArr[i][j];

					if (leftArr[i][j] >= 10)
						text.setCharacterSize(15);
					else
						text.setCharacterSize(24);

					text.setPosition(X + 2, Y + 2);
					text.setString(matrixItem.str());
					window->draw(text);
				}



			}
		}

	}

	void DrawTopMatrix(RenderWindow *window)
	{
		/*Рисуем верхнюю матрицу*/
		for (int i = 0; i < topM; i++) {
			for (int j = 0; j < topN; j++) {
				int X = (i + 5 + leftN) * SizeCell;
				int Y = j * SizeCell;
				matrixSprite.setPosition(X, Y);
				window->draw(matrixSprite);

			}
		}

		for (int i = 0; i < topM; i++) {
			int len = length_arr(topArr[i], topM);

			for (int j = 0; j < topN; j++) {
				int X = (i + 5 + leftN) * SizeCell;
				int Y = (j + topN - len) * SizeCell;

				if (topArr[i][j]){
					std::ostringstream matrixItem;
					matrixItem << topArr[i][j];

					if (topArr[i][j] >= 10)
						text.setCharacterSize(15);
					else
						text.setCharacterSize(24);

					text.setPosition(X + 2, Y + 2);
					text.setString(matrixItem.str());
					window->draw(text);
				}

			}
		}

	}

	void Click(RenderWindow *window, Event event, int st)
	{
		int xpos = Mouse::getPosition(*window).x / SizeCell;
		int ypos = Mouse::getPosition(*window).y / SizeCell;

		/*Меняем цвет клеток по клику*/
		if (m && n && xpos >= 5 + leftN && ypos >= topN) {
			change_color(event.mouseButton.button, xpos - 5 - leftN, ypos - topN);
		}
		else if (xpos < 5) {

			/*OPEN CREATE*/
			if (st == CREATOR && ypos >= 0 && ypos < 2) {
				Press(0);
				int X = (leftN + m + 5)*SizeCell;
				int Y = (n + topN)*SizeCell;
				window->create(sf::VideoMode(X, Y), "Nonagram", sf::Style::Close);
			}

			/*OPEN SOLVE*/
			if (st == SOLVE && ypos >= 0 && ypos < 2) {
				Press(4);
				int X = (leftN + m + 5)*SizeCell;
				int Y = (n + topN)*SizeCell;
				window->create(sf::VideoMode(X, Y), "Nonagram", sf::Style::Close);
			}

			/*LOAD*/
			if (ypos > 1 && ypos < 4){
				Press(1);
				int X = (leftN + m + 5)*SizeCell;
				int Y = (n + topN)*SizeCell;
				window->create(sf::VideoMode(X, Y), "Nonagram", sf::Style::Close);
			}

			/*CLEAR*/
			if (ypos > 3 && ypos < 6)
				Press(2);

			/*SOLVE*/
			if (st == SOLVE && ypos > 5 && ypos < 8)
				Press(3);

			if (st == SOLVE && ypos >= 8 && ypos <= 10)
				Press(5);

		}

	}

};

class Menu
{
private:
	enum States {MAIN, CREATE, SOLVE, CREATOR} state;

	Image image;
	Texture texture;
	Sprite sprite;

	Font font;
	Text text;

	void MenuCreator(RenderWindow *window)
	{
		int x = 0 * SizeCell;
		int y = 0 * SizeCell;
		int n = 3;

		std::string titles[] = {"Load", "Save", "Clear"}; // Заголовки
		text.setCharacterSize(24);
		sprite.setScale((float)1/3, (float)1/3);

		for (int i = 0; i < n; i++) {
			sprite.setPosition(x * SizeCell, y * SizeCell);
			text.setString(titles[i]);
			text.setPosition( (x + 1) * SizeCell, (y + 0.75) * SizeCell);
			y += 2;

			window->draw(sprite);
			window->draw(text);
		}

	}

	void MenuSolve(RenderWindow *window)
	{
		int x = 0 * SizeCell;
		int y = 0 * SizeCell;
		int n = 5;

		std::string titles[] = {"Load", "Save", "Clear", "Solve", "Check"}; // Заголовки
		text.setCharacterSize(24);
		sprite.setScale((float)1/3, (float)1/3);

		for (int i = 0; i < n; i++) {
			sprite.setPosition(x * SizeCell, y * SizeCell);
			text.setString(titles[i]);
			text.setPosition( (x + 1) * SizeCell, (y + 0.75) * SizeCell);
			y += 2;

			window->draw(sprite);
			window->draw(text);
		}
	}

	void MainMenu(RenderWindow *window)
	{
		std::string titles[] = {"Solve", "Create", "Exit"}; // Заголовки
		MenuTemplate(window, 3, 0, 0, titles);
	}

	void InputMenu(RenderWindow *window)
	{
		int x = 0 * SizeCell, y = 0 * SizeCell; // Положение спрайта
		int n = 12; // Количество кнопок

		std::string titles[] = {"10x10", "10x15", "10x20",
														"15x30", "20x20", "20x25",
														"20x30", "25x20", "25x25",
														"25x30", "30x15", "30x20"};

		text.setCharacterSize(40);
		sprite.setScale(1,1);

		for (int i = 0; i < n; i++) {
			sprite.setPosition(x, y);
			text.setString(titles[i]);
			text.setPosition( x + 2.5 * SizeCell, y + 2.5 * SizeCell);
			y += 6 * SizeCell;

			if (i % 4 == 3) {
				y = 0 * SizeCell;
				x += 15 * SizeCell;
			}


			window->draw(sprite);
			window->draw(text);
		}

	}

	void MenuTemplate(RenderWindow *window, int n, int x, int y, std::string titles[])
	{
		x *= SizeCell, y *= SizeCell; // Положение спрайта
		sprite.setScale(1, 1);

		for (int i = 0; i < n; i++) {
			sprite.setPosition(x, y);
			text.setString(titles[i]);
			text.setPosition( x + 1.4 * SizeCell, y + 2.5 * SizeCell);
			y += 6 * SizeCell;

			window->draw(sprite);
			window->draw(text);
		}

	}

public:
	int w, h;

	Menu()
	{
		w = 15 * SizeCell; h = 6 * 3 * SizeCell;
		state = MAIN;

		image.loadFromFile("images/MenuItem.jpg");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));

		font.loadFromFile("Font.ttf");
		text.setFont(font);
		text.setCharacterSize(70);
		text.setColor(Color::White);
	}

	void MenuDraw(RenderWindow *window)
	{
		switch (state) {
			case MAIN:
				MainMenu(window);
				break;
			case CREATE:
				InputMenu(window);
				break;
			case SOLVE:
				MenuSolve(window);
				break;
			case CREATOR:
				MenuCreator(window);
				break;
		}

	}

	void Click(RenderWindow *window, FieldCreator *cr, Event event)
	{
		int xpos = Mouse::getPosition(*window).x / SizeCell;
		int ypos = Mouse::getPosition(*window).y / SizeCell;

		switch (state) {
			case MAIN:
			{
					if (ypos >= 0 && ypos < 6)
						Press(0, window);
					if (ypos > 6 && ypos < 12)
						Press(1, window);
					if (ypos > 12 && ypos < 18)
						Press(2, window);
			}
				break;
			case CREATE:
			{
				if (xpos >= 0 && xpos < 15)
				{
					if (ypos >= 0 && ypos < 6)
						cr->SetMandN(10, 10);
					if (ypos > 6 && ypos < 12)
						cr->SetMandN(10, 15);
					if (ypos > 12 && ypos < 18)
						cr->SetMandN(10, 20);
					if (ypos > 18 && ypos < 24)
						cr->SetMandN(15, 30);
				}

				if (xpos > 15 && xpos < 30)
				{
					if (ypos >= 0 && ypos < 6)
						cr->SetMandN(20, 20);
					if (ypos > 6 && ypos < 12)
						cr->SetMandN(20, 25);
					if (ypos > 12 && ypos < 18)
						cr->SetMandN(20, 30);
					if (ypos > 18 && ypos < 24)
						cr->SetMandN(25, 20);
				}

				if (xpos > 30 && xpos < 45)
				{
					if (ypos >= 0 && ypos < 6)
						cr->SetMandN(25, 25);
					if (ypos > 6 && ypos < 12)
						cr->SetMandN(25, 30);
					if (ypos > 12 && ypos < 30)
						cr->SetMandN(30, 30);
					if (ypos > 18 && ypos < 15)
						cr->SetMandN(30, 20);
				}

				state = CREATOR;
				window->create(sf::VideoMode ( (cr->m + 5) * SizeCell, cr->n * SizeCell), "Creator", sf::Style::Close);
			}
				break;
			case SOLVE:
			{
				cr->Click(window, event, state);
			}
				break;
			case CREATOR:
			{
				cr->Click(window, event, state);
			}
				break;
		}

	}

	void Press(int button, RenderWindow *window)
	{
		switch (button) {
			/*SOLVE*/
			case 0:
			{
				window->create(sf::VideoMode(5*SizeCell, 2*5*SizeCell), "Nonagram", sf::Style::Close);
				window->setPosition(Vector2i(600, 237));
				state = SOLVE;
			}
				break;

			/*CREATE*/
			case 1:
			{
				w = 45 * SizeCell, h = 24 * SizeCell;
				window->create(sf::VideoMode(w, h), "Create", sf::Style::Close);
				window->setPosition(Vector2i(600, 237));
				state = CREATE;
			}
				break;

			/*EXIT*/
			case 2:
			{
				window->close();
			}
				break;

			default:
				break;
		}
	}

};

int main()
{
	int m = 0, n = 0;
	FieldCreator creator(m, n);

	/*Создаем меню*/
	Menu menu;
	RenderWindow window(sf::VideoMode(menu.w, menu.h), "Main Menu", sf::Style::Close);
	window.setPosition(Vector2i(600, 237));

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event)) {
			if(event.type == sf::Event::Closed)
				window.close();

			if(event.type == Event::MouseButtonPressed) {
				menu.Click(&window, &creator, event);
			}


		}//while event

		window.clear();

		menu.MenuDraw(&window);

		creator.DrawTopMatrix(&window);
		creator.DrawLeftMatrix(&window);
		creator.DrawField(&window);

		window.display();
	}
	return 0;
}
