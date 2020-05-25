/*
 * main.cc
 *
 *  Created on: 25 мая 2020 г.
 *      Author: 1234
 */
#include <vector>
#include <cstdint>
#include <random>
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>


class Memory
{
protected:
	std::vector<uint8_t> _contents;
public:
	virtual ~Memory() = default;

	virtual void write(unsigned addr, uint8_t value) = 0;
	virtual uint8_t read(unsigned addr) { return _contents[addr % _contents.size()]; }
};

class RAM: public Memory
{
public:
	RAM(unsigned size) { _contents.resize(size); }

	virtual void write(unsigned addr, uint8_t value) override { _contents[addr] = value; }
};

class ROM: public Memory
{
public:
	ROM(const char * filename) {
		std::fstream romfile;
		romfile.open(filename, std::ios::in | std::ios::ate);
		_contents.resize(romfile.tellg());
		romfile.seekg(0);
		romfile.read(reinterpret_cast<char*>(&_contents[0]), _contents.size());
	}

	virtual void write(unsigned addr, uint8_t value) override { }
};

bool test_RAM(RAM & ram)
{
	// Тест оперативки
	std::vector<uint8_t> testcase;
	testcase.resize(65536);
	std::minstd_rand rnd;
	for (auto && v : testcase) { v = rnd() & 0xff; }

	for (unsigned i = 0; i < 65536; ++i)
		ram.write(i, testcase[i]);

	for (unsigned i = 0; i < 65536; ++i)
		if (ram.read(i) != testcase[i]) {
			return false;
		}
	return true;
}

bool test_ROM()
{
	ROM rom { "zx.rom" };

	rom.write(0, 0x00);

	if (rom.read(0) != 0xf3) return false;
	if (rom.read(1) != 0xaf) return false;
	if (rom.read(2) != 0x32) return false;
	if (rom.read(3) != 0x3b) return false;

	if (rom.read(65532) != 0xa1) return false;
	if (rom.read(65533) != 0x99) return false;
	if (rom.read(65534) != 0x42) return false;
	if (rom.read(65535) != 0x3c) return false;

	return true;
}

int main(int, char **)
{
	RAM ram { 65536 };

	if (not test_RAM(ram)) {
		std::cerr << "А не работает у нас память!" << std::endl;
		return 1;
	}

	if (not test_ROM()) {
		std::cerr << "ПЗУ не работает как положено!" << std::endl;
	}
		std::fstream srcfile;
		srcfile.open( "test1.src", std::ios::in | std::ios::ate);
		std::vector <uint8_t> data;
		data.resize(srcfile.tellg());
		srcfile.seekg(0);
		srcfile.read(reinterpret_cast<char*> (&data[0]), data.size());
		for(unsigned i =0;i<data.size(); i++)
			ram.write(16384+i,data[i]);

	SDL_Init(SDL_INIT_EVERYTHING);
	auto win = SDL_CreateWindow("Эмулятор ZX Spectrum",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			935, 700,
			SDL_WINDOW_SHOWN);
	if (win == nullptr) { std::cerr << "Аааааа!" << std::endl; return 1; }
	auto ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == nullptr) { std::cerr << "Бббббб!" << std::endl; return 1; }

	SDL_Event event;
	for (;;) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) goto the_end;
		}

		SDL_SetRenderDrawColor(ren, 168, 168, 168, 255);
		SDL_RenderClear(ren);

		SDL_Rect screen_area_template { 92, 75, 150*5, 110*5 };
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderFillRect(ren, &screen_area_template);

		for (unsigned row = 0; row < 192; ++row) {
			for (unsigned col = 0; col < 32; ++col) {
				unsigned point = 32 * row + col;

				unsigned a4a0 = point & 0x0010f;
				unsigned a10a8 = (point>>8) & 0x0007;
				unsigned a7a5 = (point>>5) & 0x0007;
				unsigned a12a11 = (point>>11) & 0x0003;

				unsigned raster_addr =
						0x4000 |
						(a12a11 <<11)|
						(a7a5 <<8)|
						(a10a8 <<5)|
						(a4a0 <<0);

				unsigned attr_addr =
						0x5800 |
						(a12a11 <<11)|
						(a7a5 <<8)|
						(a10a8 <<5)|
						(a4a0 <<0);


				uint8_t val = ram.read(raster_addr);
				uint8_t attr = ram.read(attr_addr);
				uint8_t paper_col = ((attr >> 3) & 0x07) | ((attr & 0x40)>>3);
				uint8_t ink_col = (attr & 0x07) | ((attr & 0x40)>>3);
				for (unsigned px = 0; px < 8; ++px) {
					int blu[16] = {   0, 168,   0, 168,   0, 168,   0, 168,
						       	      0, 255,   0, 255,   0, 255,   0, 255, };
					int red[16] = {   0,   0, 168, 168,   0,   0, 168, 168,
						       	      0,   0, 255, 255,   0,   0, 255, 255, };
					int gre[16] = {   0,   0,   0,   0, 168, 168, 168, 168,
						       	      0,   0,   0,   0, 255, 255, 255, 255, };

					SDL_Rect pixel {
						int(92 + 5 * 4.8 * col + 5 * px),
						int(75 + 3 * row),
						5, 5
					};
					if ((1 << (7 - px)) & val)
						SDL_SetRenderDrawColor(ren,
								red[ink_col], gre[ink_col], blu[ink_col], 255);
					else
						SDL_SetRenderDrawColor(ren,
								red[paper_col], gre[paper_col], blu[paper_col], 255);
					SDL_RenderFillRect(ren, &pixel);

				}

			}
		}

		SDL_RenderPresent(ren);
	}
the_end:	// FIXME: Отрефакторить, и чем скорее - тем лучше!!!!


	std::cout << "Всё отлично работает." << std::endl;

	return 0;
}
