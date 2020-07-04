#include "MicroBit.h"

MicroBit uBit;

static uint8_t exposure_note[] = {
	0x6f, 0xfd,
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	0,1,2,3
};

static uint8_t advertising = 0;
static uint8_t txpower = MICROBIT_BLE_POWER_LEVELS;

static void update_txpower(int inc) {
	if(inc) {
		txpower+=inc;
		txpower = (txpower <= MICROBIT_BLE_POWER_LEVELS) ? txpower : 0;
	}
	uBit.bleManager.setTransmitPower(txpower);
	uBit.display.printCharAsync(advertising ? ('1'+txpower) : '.');
}

static void update_beacon(BLEDevice* ble) {
	uint32_t i;
	
	if(advertising)
		ble->gap().stopAdvertising();
	
    ble->clearAdvertisingPayload();

    ble->setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);
    ble->setAdvertisingInterval(250);

	for(i=2;i<sizeof(exposure_note);i++)
		exposure_note[i] = uBit.random(UINT8_MAX+1);

	/* prepending the flags somehow kills the advertisements!? */
    //ble->accumulateAdvertisingPayload(GapAdvertisingData::LE_GENERAL_DISCOVERABLE | GapAdvertisingData::SIMULTANEOUS_LE_BREDR_C | GapAdvertisingData::SIMULTANEOUS_LE_BREDR_H);
	ble->accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, exposure_note, 2);
    ble->accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, exposure_note, sizeof(exposure_note));

	update_txpower(0);
    if(advertising)
		ble->gap().startAdvertising();
}

static void startAdvertising() {
	uBit.ble->gap().startAdvertising();
	advertising = 1;
	uBit.display.printCharAsync('1'+txpower);
}

static void stopAdvertising() {
    uBit.bleManager.stopAdvertising();
    advertising = 0;
    uBit.display.printCharAsync('.');
}

void onClick(MicroBitEvent e) {
	if (e.source == MICROBIT_ID_BUTTON_A) {
		if(!advertising)
			startAdvertising();
		else
			update_txpower(1);
	}
}

void onLongClick(MicroBitEvent e) {
	if (e.source == MICROBIT_ID_BUTTON_A) {
		stopAdvertising();
	}
}

int main() {
    // Initialise the micro:bit runtime.
    uBit.init();
	uBit.seedRandom();
	update_beacon(uBit.ble);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onClick);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, onLongClick);

    startAdvertising();

    release_fiber();
}
