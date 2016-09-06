#include <cstddef>
#include <cstdint>
#include "hFramework.h"
#include "hCloudClient.h"

#define power 500       //power of motor
#define length 6500     //length of blind window in encoder tics
#define dir -1          //direction of rotation of motor (for calibration)
#define n_of_buttons 10 //number of buttons (change in code)
#define offset 460      //offset for end-point

int act_pos, des_pos;   //actual position, desired position

void cfgHandler()                               //UI
{
	auto btn1 = platform.ui.button("btn1");
	auto btn2 = platform.ui.button("btn2");

	platform.ui.loadHtml({Resource::WEBIDE, "/ui.html"});
}

void calibration_task()
{
	hMot3.setPower(500);
	sys.delay(2000);
	hMot3.setPower(dir * (power * 0.7));    //up
	hSens1.pin1.setIn_pu();
	hSens1.pin1.interruptOn_EdgeFalling();
	hSens1.pin1.interruptWait();            //end-point
	hMot3.setPower(0);                      //stop
	hMot3.resetEncoderCnt();                //reset encoder
	act_pos = hMot3.getEncoderCnt();
	hMot3.rotAbs(-1050,500);			  //down
}

void onKeyEvent(KeyEventType type, KeyCode code)
{
	switch (code) {
	case KeyCode::Key_P:
		if (type == KeyEventType::Pressed) {
			hMot3.rotRel(100);
			platform.printf("enc = %d\r\n", hMot3.getEncoderCnt());
		}
		break;
	case KeyCode::Key_L:
		if (type == KeyEventType::Pressed) {
			hMot3.rotRel(-100);
			platform.printf("enc = %d\r\n", hMot3.getEncoderCnt());
		}
		break;
	default : break;
	}
}

#define OFFSET -600
#define MAXI 7600

void onValueChangeEvent(hId id, const char* data)
{
	int val = atoi(data);
	hMot3.rotAbs(map(val, 0, 1000, -1050, 7000), 700);
}

void hMain()
{
	act_pos = 0;
	des_pos = 500;
	platform.begin(&RPi);
	platform.ui.setProjectId("d520f805929ddb0a");
	platform.ui.configHandler = cfgHandler;
	platform.ui.onKeyEvent = onKeyEvent;
	platform.ui.onValueChangeEvent = onValueChangeEvent;
	platform.ui.onButtonEvent = onButtonEvent;
	sys.taskCreate(calibration_task);               //calibration at the beginning

	for (;;) {
		sys.delay(1000);
		LED2.toggle();
		act_pos = hMot3.getEncoderCnt();
		platform.ui.label("l1").setText("Enc: %u Des_pos: %u", (unsigned int)act_pos, (unsigned int)des_pos);

	}
}
