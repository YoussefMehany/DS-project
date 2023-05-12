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