#pragma once
enum ProcessState {
	NEW,
	RDY,
	RUN,
	BLK,
	TRM,
	ORPH
};
enum ProcessorState {
	BUSY,
	IDLE
};