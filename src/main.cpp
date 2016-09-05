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
//     auto l = platform.ui.label("l1");
//     l.setText("Enc: ");
//     auto b0 = platform.ui.button("b0");      //buttons as positions (from top to bottom)
// 	auto b1 = platform.ui.button("b1");    
// 	auto b2 = platform.ui.button("b2");    
// 	auto b3 = platform.ui.button("b3");     
// 	auto b4 = platform.ui.button("b4");    
// 	auto b5 = platform.ui.button("b5");   
// 	auto b6 = platform.ui.button("b6");   
// 	auto b7 = platform.ui.button("b7");   
// 	auto b8 = platform.ui.button("b8");   
// 	auto b9 = platform.ui.button("b9");   
	
// 	auto c = platform.ui.button("cal");     //calibration button
// 	c.setText("Calibrate");
// 	c.setLocation(300,100);
// 	b0.setLocation(100,100);
// 	b1.setLocation(100,200);
// 	b2.setLocation(100,300);
// 	b3.setLocation(100,400);
// 	b4.setLocation(100,500);
// 	b5.setLocation(100,600);
// 	b6.setLocation(100,700);
// 	b7.setLocation(100,800);
// 	b8.setLocation(100,900);
// 	b9.setLocation(100,1000);
	
	platform.ui.loadHtml({Resource::WEBIDE, "/ui.html"});
}

void calibration_task()
{
    hMot3.setPower(dir*(power*0.7));           //up
    hSens1.pin1.setIn_pu();
    hSens1.pin1.interruptOn_EdgeFalling();
	hSens1.pin1.interruptWait();            //end-point
	hMot3.setPower(0);                      //stop
	hMot3.resetEncoderCnt();                //reset encoder
	act_pos=hMot3.getEncoderCnt();          
	while (act_pos<=offset)                    //100 enc tics under end-point      
	{
	    hMot3.setPower((-dir)*power*0.7);   //down 
	    act_pos=hMot3.getEncoderCnt();
	}
	hMot3.setPower(0);                      //stop
    	
}
void movement_task()
{
    while(act_pos!=des_pos)
    {
        act_pos=hMot3.getEncoderCnt();
        if(act_pos<des_pos)
        {
            hMot3.setPower((-dir)*power);
        }
        else if(act_pos>des_pos)
        {
            hMot3.setPower((dir)*power);
        }
    }
    hMot3.setPower(0);
}

void onButtonEvent(hId id, ButtonEventType type)
{
	if (id == "calibrate") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			sys.taskCreate(calibration_task);
			LED1.on();
		}
	}
	if (id == "b0") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(0*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b1") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(1*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b2") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(2*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b3") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(3*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b4") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(4*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b5") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(5*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b6") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(6*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b7") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(7*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b8") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(8*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
	if (id == "b9") 
	{
		if (type == ButtonEventType::Pressed) 
		{
			des_pos=offset+(9*(length/n_of_buttons));
			sys.taskCreate(movement_task);
		}
	}
}

void hMain()
{
    act_pos=0;
    des_pos=500;
	platform.begin(&RPi);
	platform.ui.setProjectId("bcbef7cb3d5aeeae");
	platform.ui.configHandler = cfgHandler;
	platform.ui.onButtonEvent = onButtonEvent;
	sys.taskCreate(calibration_task);               //calibration at the beginning

	for (;;) 
	{
	    sys.delay(1000);
	    LED2.toggle();
		act_pos=hMot3.getEncoderCnt();
		platform.ui.label("l1").setText("Enc: %u Des_pos: %u", (unsigned int)act_pos, (unsigned int)des_pos);
		//sys.taskCreate(movement_task);
	}
}
