/*
 * main.cc
 *
 *  Created on: 25 мая 2020 г.
 *      Author: 1234
 */
/* Суть TDD:
 *
 * 1. Пишем код, который не компилируется / не работает
 * 2. Пишем тест к этому коду - тест проваливается
 * 3. Делаем так, чтоб тест не проваливался, реализуя
 *		_минимальный_ функционал для этого
 * 4. Прогоняем тесты, убеждаемся что всё работает
 * 5. Делаем рефакторинг
 * 6. Прогоняем тесты ещё раз, убеждаемся, что не сломали
 * 7. Переходим к пункту 1
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
 * +  ОЗУ
 * +  ПЗУ
 * +- Видеовыход		(не хватает атрибута FL)
 * -  Процессор
 * -  Клава
 * +- Несколько портов	(пока только рамка)
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
