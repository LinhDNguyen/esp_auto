#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <ControlCommand.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR
user_rf_pre_init(void)
{
}
#ifdef __cplusplus
}
#endif

ControlCommand exampleCommand;

void processApplicationCommands(String commandLine, CommandOutput* commandOutput)
{
	commandOutput->printf("This command is handle by the application\r\n");
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default

	// commandHandler.registerSystemCommands();
	Serial.systemDebugOutput(false); // Enable debug output to serial
	Serial.commandProcessing(true);
	WifiAccessPoint.enable(false);
	WifiStation.enable(false);


	exampleCommand.initCommand();
	commandHandler.registerCommand(CommandDelegate("example","Example Command from Class","Application",processApplicationCommands));
}
