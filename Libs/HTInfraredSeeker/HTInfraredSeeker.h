/*
  HTInfraredSeeker.h - A library for the HiTechnic IRSeeker V2 infrared sensor.
  Created by Birk Blechschmidt, August 1, 2013.
  Released into the public domain.
*/
#ifndef HTInfraredSeeker_h
	#define HTInfraredSeeker_h
	#include "Arduino.h"

	struct InfraredResult
	{
	  byte Direction;
	  byte Strength;
	};

	class InfraredSeeker
	{
	  public:
		static void Initialize();
		static boolean Test();
		static void ReadACRaw(byte* buffer);
		static void ReadDCRaw(byte* buffer);
		static InfraredResult ReadAC();
		static InfraredResult ReadDC();
		static int DirectionAngle(byte Direction);
	  private:
		static InfraredResult PopulateValues(byte* buffer);
		static void ReadValues(byte OffsetAddress, byte* buffer);
		static const int Address = 0x10 / 2; //Divide by two as 8bit-I2C address is provided
	};
#endif