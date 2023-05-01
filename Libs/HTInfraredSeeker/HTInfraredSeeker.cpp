/*
  HTInfraredSeeker.cpp - A library for the HiTechnic IRSeeker V2 infrared sensor.
  Created by Birk Blechschmidt, August 1, 2013.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Wire.h"
#include "HTInfraredSeeker.h"

void InfraredSeeker::Initialize()
{
	Wire.begin();
	Wire.beginTransmission(InfraredSeeker::Address);
	Wire.write(0x00);
	Wire.endTransmission();
	while(Wire.available() > 0)
	Wire.read();
}

boolean InfraredSeeker::Test()
{
	Wire.beginTransmission(InfraredSeeker::Address);
	Wire.write(0x08);
	Wire.endTransmission();
	Wire.requestFrom(InfraredSeeker::Address, 16);
	char Manufacturer_Model[16];
	while(Wire.available() < 16);
	for(byte i=0; i < 16; i++)
	{
		Manufacturer_Model[i] = Wire.read();
		Serial.write(Manufacturer_Model[i]);
	}
	while(Wire.available() > 0)
	Wire.read();
	return strncmp(Manufacturer_Model, "HiTechncNewIRDir", 16) == 0;
}

void InfraredSeeker::ReadValues(byte OffsetAddress, byte* buffer)
{
	Wire.beginTransmission(InfraredSeeker::Address);
	Wire.write(OffsetAddress);
	Wire.endTransmission();
	Wire.requestFrom(InfraredSeeker::Address, 6);
	while(Wire.available() < 6);
	for(byte i = 0; i < 6; i++)
	{
		buffer[i] = Wire.read();
	}
	while(Wire.available() > 0)
	Wire.read();
}

void InfraredSeeker::ReadACRaw(byte* buffer)
{
	ReadValues(0x49, buffer);
}

void InfraredSeeker::ReadDCRaw(byte* buffer)
{
	ReadValues(0x42, buffer);
}

InfraredResult InfraredSeeker::PopulateValues(byte* buffer)
{
	InfraredResult Data;
	Data.Direction = buffer[0];
	if(buffer[0] != 0)
	{
		if(buffer[0] % 2 == 0)
		{
			Data.Strength = (buffer[buffer[0] / 2] + buffer[buffer[0] / 2 + 1]) / 2;
		}
		else
		{
			Data.Strength = buffer[buffer[0] / 2 + 1];
		}
	}
	else
	{
		Data.Strength = 0;
	}
	return Data;
}

InfraredResult InfraredSeeker::ReadAC()
{
	byte buffer[6];
	ReadACRaw(buffer);
	return PopulateValues(buffer);
}

InfraredResult InfraredSeeker::ReadDC()
{
	byte buffer[6];
	ReadDCRaw(buffer);
	return PopulateValues(buffer);
}

int InfraredSeeker::DirectionAngle(byte Direction)
{
	return Direction * 30 - 150;
}