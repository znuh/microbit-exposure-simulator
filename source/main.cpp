#include "MicroBit.h"

MicroBit uBit;

static uint8_t exposure_note[] = {
	0x6f, 0xfd,
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	0,1,2,3
};

static uint8_t randomize_txpower = 0;
static uint8_t advertising = 0;
static uint8_t txpower = MICROBIT_BLE_POWER_LEVELS - 1;

static void update_txpower(int inc) {
	if(inc) {
		txpower+=inc;
		txpower = (txpower < MICROBIT_BLE_POWER_LEVELS) ? txpower : 0;
	}
	uBit.bleManager.setTransmitPower(txpower);
	uBit.display.printCharAsync(advertising ? ('1'+txpower) : '.');
}

static void update_beacon(BLEDevice* ble) {
	uint32_t i;
	
	if(advertising)
		ble->gap().stopAdvertising();

    /* get a fresh bdaddr as well */
    ble->gap().setAddress(BLEProtocol::AddressType::RANDOM_PRIVATE_RESOLVABLE, {0});

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
	if (e.source == MICROBIT_ID_BUTTON_A)
		stopAdvertising();
	if (e.source == MICROBIT_ID_BUTTON_B)
		randomize_txpower ^= 1;
}

int main() {
	uint32_t next_rpi_change, now = uBit.systemTime();

    uBit.seedRandom(); /* needs to be done before uBit.init() or the seed will always be the same - WTF?! */

    uBit.init();

	update_beacon(uBit.ble);
	next_rpi_change = now + (600 + uBit.random(600))*1000;

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onClick);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_LONG_CLICK, onLongClick);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_LONG_CLICK, onLongClick);

    startAdvertising();

    while(1) {
		now = uBit.systemTime();

		/* change RPI + AEM every 10..20 minutes */
		if(now >= next_rpi_change) {
			update_beacon(uBit.ble);
			next_rpi_change = now + (600 + uBit.random(600))*1000;
		}

		if((randomize_txpower) && (advertising)) {
			txpower = uBit.random(MICROBIT_BLE_POWER_LEVELS);
			update_txpower(0);
		}

		uBit.sleep(500);
	}

    release_fiber();
}
