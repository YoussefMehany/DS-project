#pragma once
enum ProcessState {
	NEW,
	RDY,
	RUN,
	BLk,
	TRm,
	ORPH
};
enum ProcessorState {
	BUSY,
	IDLE
};