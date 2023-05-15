#pragma once
enum ProcessState {
	NEW,
	RDy,
	RUn,
	BLk,
	TRm,
	ORPH
};
enum ProcessorState {
	BUSY,
	IDLE,
	STOP
};
enum InterfaceMode 
{
	Interactive,
	SBS,
	Silent
};
enum Colors
{
	RED,
	YELLOW,
	WHITE,
	BOLDMAG,
	MAGENTA,
	ORANGE,
	GREEN,
	CYAN,
	BLUE
};