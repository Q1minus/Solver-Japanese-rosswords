#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace sf;

int **init_arr(int n, int m)//n - размер массива с массивами, размер m - массива с данными;
{
  int i = 0, j = 0;
  int **arr = new int *[n];

  for (i = 0; i < n; i++){
    arr[i] = new int [m];
  }

  for (i = 0; i < n; i++){
    for (j = 0; j < m; j++){
      arr[i][j] = 0;
    }
  }

  return arr;
}

void destroy_arr(int **arr, int n, int m)
{
  for (int i = 0; i < 10; i++){
    delete [] arr[i];
  }
  delete [] arr;
}

void show_arr(int **arr, int n, int m)
{
  int i = 0, j = 0;

  for (i = 0; i < n; i++){
    for (j = 0; j < m; j++){
      std::cout << arr[i][j] << " ";
    }
      std::cout << std::endl;
  }
}

class JC{
protected:
  int SizeCell;
  int Mn, Nm; // Mn - size VertData(cell); Nm - size HorData(cell);
public:
	int m, n; // m - heigth, n - width;
  int **VertData, **HorData;

  int return_Nm(){
    return (n / 2 + 1);
  }

  int return_Mn(){
    return (m / 2 + 1);
  }

  void init_n_and_m(int a, int b){
    n = a;
    m = b;
    SizeCell = 64;
    Nm = n / 2 + 1;
    Mn = m / 2 + 1;
  }

  int **user_imput_HorData(int n, int Mn) // n - width, Mn - the size of the array data, m - width;
  {
    int **arr = init_arr(n, Mn);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < Mn; j++) {
        std::cout << "a[" << i << "][" << j << "] = ";
        std::cin >> arr[i][j];
      }
    }
    //show_arr(arr, n, Mn);
    return arr;
  }

};

class HorData : public JC{
public:
  Sprite HorDataSprite;

  void hor_data_position(int x, int y) {
    HorDataSprite.setPosition(64*x, 64*(y+2);
  }

};

class VertData : public JC{
public:
  Sprite VertDataSprite;

  void vert_data_position(int x, int y) {
		VertDataSprite.setPosition((x + 2)* 64, y* 64);
	}

};

int main()
{
  JC nng;
  nng.init_n_and_m(3, 4);
  int Nm = nng.return_Nm();
  int Mn = nng.return_Mn();
	RenderWindow window(sf::VideoMode(64*(2 + 3), 64*(2 + 4)), "TEST", Style::Close);
  Texture DataTexture;
	DataTexture.loadFromFile("q/datacell.jpg");

  VertData vd[3][2];
  HorData hd[2][4];

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++)  {
      vd[i][j].vert_data_position(i, j);
      vd[i][j].VertDataSprite.setTexture(DataTexture);
      vd[i][j].VertDataSprite.setTextureRect(IntRect(0, 0, 64, 64));
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++)  {
      hd[i][j].hor_data_position(i, j);
      hd[i][j].HorDataSprite.setTexture(DataTexture);
      hd[i][j].HorDataSprite.setTextureRect(IntRect(0, 0, 64, 64));
    }
  }


	while (window.isOpen())	{
		Event event;
		while (window.pollEvent(event)){
			switch (event.type){
				case Event::Closed:
					window.close();
					break;

				default:
					break;
			}
		}

    window.clear();

    for (int i = 0; i < 3; i++){
			for (int j = 0; j < 2; j++){
				window.draw(vd[i][j].VertDataSprite);
			}
		}

    for (int i = 0; i < 2; i++){
			for (int j = 0; j < 4; j++){
				window.draw(hd[i][j].HorDataSprite);
			}
		}

		window.display();
	}
	return 0;
}
