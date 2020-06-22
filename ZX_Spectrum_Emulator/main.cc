/*
 * main.cc
 *
 *  Created on: 25 ��� 2020 �.
 *      Author: 1234
 */
/* ���� TDD:
 *
 * 1. ����� ���, ������� �� ������������� / �� ��������
 * 2. ����� ���� � ����� ���� - ���� �������������
 * 3. ������ ���, ���� ���� �� ������������, ��������
 *		_�����������_ ���������� ��� �����
 * 4. ��������� �����, ���������� ��� �� ��������
 * 5. ������ �����������
 * 6. ��������� ����� ��� ���, ����������, ��� �� �������
 * 7. ��������� � ������ 1
 *
 */

#include <vector>
#include <cstdint>
#include <random>
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>

#include "Window.h"

/*
 * +  ���
 * +  ���
 * +- ����������		(�� ������� �������� FL)
 * -  ���������
 * -  �����
 * +- ��������� ������	(���� ������ �����)
 *
 */

int main(int, char **)
{

	SDL_Init(SDL_INIT_EVERYTHING);

	try {
		Window win;

		win.main();
	} catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "An unknown exception has occured!" << std::endl;
		return 2;
	}

	return 0;
}
