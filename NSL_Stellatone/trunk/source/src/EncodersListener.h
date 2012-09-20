/*
 * Copyright 2011 Xavier Hosxe
 *
 * Author: Xavier Hosxe (xavier.hosxe@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef ENCODERSLISTENER_H_
#define ENCODERSLISTENER_H_

class EncodersListener {
public:
	virtual void encoderTurned(int num, int ticks) = 0;
	virtual void buttonPressed(int number) = 0;
	virtual void buttonLongPressed(int number) = 0;
	EncodersListener* nextListener;

};

#endif /* ENCODERSLISTENER_H_ */