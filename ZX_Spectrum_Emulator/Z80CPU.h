/*
 * Z80CPU.h
 *
 *  Created on: 26 мая 2020 г.
 *      Author: 1234
 */

#ifndef Z80CPU_H_
#define Z80CPU_H_

#include "Memory.h"
#include <z80.h>
#include <libspectrum.h>

class Z80CPU
{
protected:
	AddressSpace & _bus;
	vz80::VZ80 _z80;

public:
	Z80CPU(AddressSpace & bus): _bus(bus) {
		_z80.set_ior8( [&](uint16_t addr) -> uint8_t { return _bus.read(addr, true); } );
		_z80.set_iow8( [&](uint16_t addr, uint8_t v) { _bus.write(addr, v, true); } );
		_z80.set_memr8( [&](uint16_t addr) -> uint8_t { return _bus.read(addr, false); } );
		_z80.set_memw8( [&](uint16_t addr, uint8_t v) { _bus.write(addr, v, false); } );
	}

	void tick() {
		_z80.exec();
	}
	void ticks(unsigned ticks) {
		_z80.exec(ticks);
	}

	void reset() {
		_z80.reset();
	}

	void intr() {
		_z80.intr(0);
	}

	void save_state_sna(const char * filename);
	void load_state_sna(const char * filename);
	void load_state_z80(const char * filename);

	void load_state_sna_libspectrum(const char * filename);
	void load_state_z80_libspectrum(const char * filename);

};




#endif /* Z80CPU_H_ */
