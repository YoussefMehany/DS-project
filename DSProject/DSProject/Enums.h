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
	IDLE
};
enum InterfaceMode 
{
	Interactive,
	SBS,
	Silent
};