#include "MicroBit.h"

MicroBit uBit;

static uint8_t exposure_note[] = {
	//0x02, 0x01, 0x1A,
	//0x03, 0x03, 0x6f, 0xfd,
	//0x17, 0x16, 
	0x6f, 0xfd,
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	0,1,2,3 };

const int8_t CALIBRATED_POWERS[] = {-49, -37, -33, -28, -25, -20, -15, -10};

static uint8_t advertising = 0;
static uint8_t tx_power_level = 6;

static void beacon_init(BLEDevice* ble) {
	//uint8_t flags = 0x1A;
	
    ble->gap().stopAdvertising();
    ble->clearAdvertisingPayload();

    ble->setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    ble->setAdvertisingInterval(250);

	/* prepending the flags somehow kills the advertisements!? */
    //ble->accumulateAdvertisingPayload(GapAdvertisingData::LE_GENERAL_DISCOVERABLE | GapAdvertisingData::SIMULTANEOUS_LE_BREDR_C | GapAdvertisingData::SIMULTANEOUS_LE_BREDR_H);
	//ble->accumulateAdvertisingPayload(GapAdvertisingData::FLAGS, &flags, sizeof(flags));
	ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, exposure_note, 2);
    ble->accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, exposure_note, sizeof(exposure_note));

    ble->gap().startAdvertising();
}

void startAdvertising() {
    beacon_init(uBit.ble);
    uBit.bleManager.setTransmitPower(tx_power_level);
    uBit.display.scroll("ADV");
    advertising = 1;
}

void stopAdvertising() {
    uBit.bleManager.stopAdvertising();
    uBit.display.scroll("OFF");
    advertising = 0;
}

void onButtonA(MicroBitEvent) {
    if (advertising == 1)
        return;
    startAdvertising();
}

void onButtonB(MicroBitEvent) {
    if (advertising == 0)
        return;
    stopAdvertising();
}

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    
    startAdvertising();
    
    release_fiber();
}
