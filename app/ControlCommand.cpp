#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <ControlCommand.h>

ControlCommand::ControlCommand()
{
	debugf("ControlCommand Instantiating");
}

ControlCommand::~ControlCommand()
{
	if (this->channels) {
		free(this->channels);
	}
}


bool ControlCommand::getChannelStatus(int index) {
	return this->channels[index].status;
}
bool ControlCommand::setChannelStatus(int index, bool val) {
	struct control_channel *channel = &this->channels[index];
	channel->status = val;
	digitalWrite(channel->pin, val);
	return channel->status;
}

void ControlCommand::initCommand()
{

	struct control_channel channels[] = {
		{ POW1_PIN, "POW1\0", false },
		{ POW2_PIN, "POW2\0", false },
		{ NET1_PIN, "NET1\0", false },
		{ NET2_PIN, "NET2\0", false },
	};

	this->channel_count = sizeof(channels)/sizeof(struct control_channel);
	this->channels = (struct control_channel *)malloc(sizeof(channels));
	memcpy(this->channels, channels, sizeof(channels));

	// Init pins
	for (int i=0; i < this->channel_count; ++i) {
		struct control_channel *channel = &this->channels[i];

		pinMode(channel->pin, OUTPUT);
		digitalWrite(channel->pin, channel->status);
	}

	commandHandler.registerCommand(CommandDelegate("channel","Control power/ethernet channel","Application",commandFunctionDelegate(&ControlCommand::processControlCommands,this)));
}

int ControlCommand::findChannelByName(String name) {
	for(int i=0; i < this->channel_count; ++i) {
		if (String(this->channels[i].name) == name) {
			return i;
		}
	}
	return -1;
}

void ControlCommand::processControlCommands(String commandLine, CommandOutput* commandOutput)
{
	Vector<String> commandToken;
	int numToken = splitString(commandLine, ' ' , commandToken);

	if ((numToken == 2) && (commandToken[1] == "status")) {
		// Print status
		for (int i = 0; i < this->channel_count; ++i) {
			struct control_channel *channel = &this->channels[i];
			commandOutput->printf("channel %s: %s\r\n", channel->name, channel->status?"ON":"OFF");
		}
	}
	else if (numToken == 3)
	{
		int index = this->findChannelByName(commandToken[1]);

		if (index < 0) {
			commandOutput->printf("ERR: channel %s is invalid\r\n", commandToken[1].c_str());
			return;
		}

		bool val = false;
		String status = commandToken[2];
		status.toLowerCase();
		if (status == "on") val = true;

		bool res = this->setChannelStatus(index, val);
		commandOutput->printf("Now channel %s has status %s\r\n", this->channels[index].name, res ? "ON":"OFF");
	} else {
		commandOutput->printf("Usage: channel <NET1|NET2|POW1|POW2> <ON|OFF>\r\n");
		commandOutput->printf("Example Commands available : \r\n");
		commandOutput->printf("NET1 ON   : Set example status ON\r\n");
		commandOutput->printf("NET1 OFF  : Set example status OFF\r\n");
		commandOutput->printf("status : Show example status\r\n");
	}
}


