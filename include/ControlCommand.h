/*
 * ControlCommand.h
 *
 */

#ifndef SMINGCORE_EXAMPLE_COMMAND_H_
#define SMINGCORE_EXAMPLE_COMMAND_H_

#include "WString.h"
#include "../Services/CommandProcessing/CommandProcessingIncludes.h"

#define POW1_PIN 5
#define POW2_PIN 4
#define NET1_PIN 14
#define NET2_PIN 12

struct control_channel {
	unsigned char pin;
	char name[10];
	bool status;
};

class ControlCommand
{
public:
	ControlCommand();
	virtual ~ControlCommand();
	void initCommand();
	bool getChannelStatus(int index);
	bool setChannelStatus(int index, bool val);

private:
	int channel_count = 0;
	struct control_channel *channels = NULL;
	void processControlCommands(String commandLine, CommandOutput* commandOutput);
	int findChannelByName(String name);
};


#endif /* SMINGCORE_DEBUG_H_ */
