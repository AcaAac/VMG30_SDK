// Test_SDK.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <windows.h>
#include "wxVTK.h"	//rendering class

//include dataglove class
#include "VMG30.h"

//arg[1] = comport, arg[2] = ipaddress
int main(int argc, char* argv[])
{
	//init rendering and skeleton structure structure
	
	//create a new dataglove instance
	VMG30HANDLE gloveH = GetVMGlove();

	//get comport and ipaddress
	char ipaddr[256];sprintf(ipaddr,"127.0.0.1");
	int comport = 1;
	if (argc>1) comport = atoi((char *)argv[1]);
	if (argc>2) sprintf(ipaddr,"%s",(char *)argv[2]);
	VMGloveSetConnPar(gloveH,comport,ipaddr);

	//uncomment these lines to update dataglove setting
	//VMGloveSetID(gloveH,"TestName",2);
	//VMGloveSetWiFiSettings(gloveH,"192.168.2.34","255.255.255.0","192.168.2.254",1);
	//VMGloveSetAPNSettings(gloveH,"ASUS","12345678");
	//VMGloveStoreSettings(gloveH);

	//change bones lenght
#if 0
	VMGloveSetBoneLenght(gloveH,"MIDDLE0",60.0);
	VMGloveSetBoneLenght(gloveH,"MIDDLE1",70.0);
	VMGloveSetBoneLenght(gloveH,"MIDDLE2",64.0);
	VMGloveSetBoneLenght(gloveH,"MIDDLE3",50.0);
#endif

	//uncomment this line to set left handed dataglove
	//VMGloveSetLeftHanded(gloveH,1);
	
	wxVTK *vtk = new wxVTK();

	//check conn parameters
	int comp;
	char ip[VHAND_STRLEN];
	VMGloveGetConnPar(gloveH,&comp,ip);
	fprintf(stderr,"COMP:%d IP:%s\n",comp,ip);

startconn:
	//stop any streaming 
	VMGloveDisconnect(gloveH);
	Sleep(500);
	//connecto to the dataglove
	VMGloveConnect(gloveH,CONN_USB,PKG_RAW_FINGER);

	int cnt = 0;
	long start = ::GetTickCount();

	//1 minute sampling
	while ((::GetTickCount()-start)<60000){
		int cs = VMGloveGetConnectionStatus(gloveH);
		Sleep(50);
		fprintf(stderr,"CONNSTATUS: %d\n",cs);
		if (cs == USB_CONNECTED){
			//get wrist attitudwe
			double roll, pitch, yaw;
			VMGloveGetAttitudeWrist(gloveH,&roll,&pitch,&yaw);
			//fprintf(stderr,"WRIST ROLL: %.1f PITCH:%.1f YAW:%.1f\n",roll,pitch,yaw);
			//get hand attitude
			VMGloveGetAttitudeHand(gloveH,&roll,&pitch,&yaw);
			//fprintf(stderr,"HAND ROLL: %.1f PITCH:%.1f YAW:%.1f\n",roll,pitch,yaw);

			//ricavo gyroH, gyroW
			double RAD2G = (180.0/3.1415);
			double gyroXH,gyroYH,gyroZH,gyroXW,gyroYW,gyroZW;
			double accXH,accYH,accZH,accXW,accYW,accZW;
			VMGloveGetGyrosHand(gloveH,&gyroXH,&gyroYH,&gyroZH);
			VMGloveGetGyrosWrist(gloveH,&gyroXW,&gyroYW,&gyroZW);
			VMGloveGetAccelsHand(gloveH,&accXH,&accYH,&accZH);
			VMGloveGetAccelsWrist(gloveH,&accXW,&accYW,&accZW);
			printf("GYROH: %.4f %.4f %.4f\n",RAD2G*gyroXH,RAD2G*gyroYH,RAD2G*gyroZH);
			printf("GYROW: %.4f %.4f %.4f\n",RAD2G*gyroXW,RAD2G*gyroYW,RAD2G*gyroZW);
			printf("ACCH: %.4f %.4f %.4f\n",accXH,accYH,accZH);
			printf("ACCW: %.4f %.4f %.4f\n",accXW,accYW,accZW);

			//get finger bendingd
			double fingers[10];
			VMGloveGetFingers(gloveH,fingers);
			int i = 0;
			for (i=0;i<5;i++){
				//fprintf(stderr,"FINGER%d: %.1f %.1f\n",i+1,fingers[2*i],fingers[2*i+1]);
			}

			double abds[4];
			VMGloveGetAbductions(gloveH,abds);
			for (i=0;i<4;i++){
				//fprintf(stderr,"ABD%d: %.1f\n",i+1,abds[i]);
			}
			//get pressure sensors status
			double press[10];
			VMGloveGetPressures(gloveH,press);
			for (i=0;i<5;i++){
				//fprintf(stderr,"PRESS%d: %.1f\n",i+1,press[i]);
			}

			double palmarch = 0.0f, thumbco = 0.0f;
			VMGloveGetPalmArch(gloveH,&palmarch);
			VMGloveGetThumbCrossOver(gloveH,&thumbco);
			//fprintf(stderr,"PALMARCH: %.2f THUMBCO:%.2f\n",palmarch,thumbco);

			//update rendering, getting nodes position from the sdk
			double x,y,z;
			VMGloveGetPosition(gloveH,"WRIST",&x,&y,&z);
			vtk->SetNodePosition("WRIST",x,y,z);
			VMGloveGetPosition(gloveH,"HAND",&x,&y,&z);
			vtk->SetNodePosition("HAND",x,y,z);

			VMGloveGetPosition(gloveH,"THUMB0",&x,&y,&z);
			vtk->SetNodePosition("THUMB0",x,y,z);
			VMGloveGetPosition(gloveH,"THUMB1",&x,&y,&z);
			vtk->SetNodePosition("THUMB1",x,y,z);
			VMGloveGetPosition(gloveH,"THUMB2",&x,&y,&z);
			vtk->SetNodePosition("THUMB2",x,y,z);
			VMGloveGetPosition(gloveH,"THUMB3",&x,&y,&z);
			vtk->SetNodePosition("THUMB3",x,y,z);

			VMGloveGetPosition(gloveH,"INDEX0",&x,&y,&z);
			vtk->SetNodePosition("INDEX0",x,y,z);
			VMGloveGetPosition(gloveH,"INDEX1",&x,&y,&z);
			vtk->SetNodePosition("INDEX1",x,y,z);
			VMGloveGetPosition(gloveH,"INDEX2",&x,&y,&z);
			vtk->SetNodePosition("INDEX2",x,y,z);
			VMGloveGetPosition(gloveH,"INDEX3",&x,&y,&z);
			vtk->SetNodePosition("INDEX3",x,y,z);

			VMGloveGetPosition(gloveH,"MIDDLE0",&x,&y,&z);
			vtk->SetNodePosition("MIDDLE0",x,y,z);
			VMGloveGetPosition(gloveH,"MIDDLE1",&x,&y,&z);
			vtk->SetNodePosition("MIDDLE1",x,y,z);
			VMGloveGetPosition(gloveH,"MIDDLE2",&x,&y,&z);
			vtk->SetNodePosition("MIDDLE2",x,y,z);
			VMGloveGetPosition(gloveH,"MIDDLE3",&x,&y,&z);
			vtk->SetNodePosition("MIDDLE3",x,y,z);

			VMGloveGetPosition(gloveH,"RING0",&x,&y,&z);
			vtk->SetNodePosition("RING0",x,y,z);
			VMGloveGetPosition(gloveH,"RING1",&x,&y,&z);
			vtk->SetNodePosition("RING1",x,y,z);
			VMGloveGetPosition(gloveH,"RING2",&x,&y,&z);
			vtk->SetNodePosition("RING2",x,y,z);
			VMGloveGetPosition(gloveH,"RING3",&x,&y,&z);
			vtk->SetNodePosition("RING3",x,y,z);

			VMGloveGetPosition(gloveH,"LITTLE0",&x,&y,&z);
			vtk->SetNodePosition("LITTLE0",x,y,z);
			VMGloveGetPosition(gloveH,"LITTLE1",&x,&y,&z);
			vtk->SetNodePosition("LITTLE1",x,y,z);
			VMGloveGetPosition(gloveH,"LITTLE2",&x,&y,&z);
			vtk->SetNodePosition("LITTLE2",x,y,z);
			VMGloveGetPosition(gloveH,"LITTLE3",&x,&y,&z);
			vtk->SetNodePosition("LITTLE3",x,y,z);

			//update fingertips colour from pressure sensors (GREEN = no pressure, RED = MAX PRESSURE)
			vtk->SetNodeColour("THUMB3",t_Colour(255-255*press[0]/100.0,255*press[0]/100.0,0));
			vtk->SetNodeColour("INDEX3",t_Colour(255-255*press[1]/100.0,255*press[1]/100.0,0));
			vtk->SetNodeColour("MIDDLE3",t_Colour(255-255*press[2]/100.0,255*press[2]/100.0,0));
			vtk->SetNodeColour("RING3",t_Colour(255-255*press[3]/100.0,255*press[3]/100.0,0));
			vtk->SetNodeColour("LITTLE3",t_Colour(255-255*press[4]/100.0,255*press[4]/100.0,0));
			
			vtk->Update();
		}
		cnt++;
	}
	VMGloveDisconnect(gloveH);
	Sleep(1000);
	//goto startconn;
}

