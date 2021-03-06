// Serial_Sunghyun.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"

#include<stdio.h>
#include<Windows.h>

u_char buffOne[2];
u_char buffPayload[256] = { 0 };
HANDLE serialHandle;
DWORD dword;

void apartOne(); // 한 바이트를 buffOne에 저장
void apartPayload(int);
void checkSync(); // Sync 바이트를 확인
int checkSum(int); // 
void analysisPayload(); // payload를 분석함
u_char checkPlength(); // plength를 체크함
void pushSpace();

int main(int argc, char ** argv) {

	serialHandle = CreateFile("COM5", // 핸들러 설정
		GENERIC_READ | GENERIC_WRITE,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (serialHandle == INVALID_HANDLE_VALUE) { // 만약에 핸들러를 제대로 가져오지 못했다면,,

		printf("(eror:6~12) you can't get HANDLE\n");
		return -1;
	}
	else {

		printf("success connect serial\n");
	}

	DCB dcbSerialParams = { 0 }; // DCB를 통해 통신 설정하기 
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	GetCommState(serialHandle, &dcbSerialParams);

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	SetCommState(serialHandle, &dcbSerialParams); // 통신 세팅

	COMMTIMEOUTS timeOut = { 0 };
	timeOut.ReadIntervalTimeout = 50;
	timeOut.ReadTotalTimeoutConstant = 50;
	timeOut.ReadTotalTimeoutMultiplier = 10;
	timeOut.WriteTotalTimeoutConstant = 50;
	timeOut.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(serialHandle, &timeOut)) { // TIMEOUT 설정
		printf("error(51) error in timeOut");
		return -1;
	}

	while (1) {

		checkSync();
		u_char plength = checkPlength();

		if (plength == -1) {
			printf("PLENGTH ERROR\n");
			continue; // plength가 깨졌을 때에 처음부터 시작
		}
		else {
			apartPayload(plength);
			if (checkSum(plength)) { // 체크된 페킷이라면 페킷 분석
				if (plength > 20) {
					analysisPayload();
				}
			}
			else { // 패킷이 체크되지 않다면 처음으로
				continue;
			}
		}
	}
	CloseHandle(serialHandle);
}

void apartOne() { // 1바이트씩 분리
	DWORD dword = 0;
	if (!ReadFile(serialHandle, buffOne, 1, &dword, NULL)) {
		printf("error in apartOne()");
	}
}

void apartPayload(int plength) { // 페이로드를 읽어 옴
	DWORD dword;
	if (!ReadFile(serialHandle, buffPayload, plength, &dword, NULL)) {

		printf("error in apartPayload()");
	}
}

void checkSync() { // SYNC 확인

	while (1) {
		apartOne();
		if (buffOne[0] == 0xAA) {
			apartOne();
			if (buffOne[0] == 0xAA) {
				break;
			}
		}
	}
}

u_char checkPlength() { // plength을 체크함

	while (1) {

		apartOne();
		u_char plength = buffOne[0];
		if (plength == 0xAA) continue;
		else if (plength > 0xAA) return -1; // 깨지면 -1 반환
		else return plength; // 아니면 plength 반환
	}
}

int checkSum(int plength) { // 체크섬 확인

	int total = 0;
	u_char checksum;

	for (int i = 0; i < plength; i++)
		total += buffPayload[i];

	checksum = ~(total & 0xFF);

	apartOne();
	if (buffOne[0] == checksum) return 1; // 맞으면 1 반환
	else return 0;
}

void analysisPayload() { // 페이로드 분석

	int i = 0;
	int vlength;

	// vlength까지의 데이터를 일어옴
	
	for (; buffPayload[i] <= 0x80; i++) {}

	vlength = buffPayload[i+1];

	i = 0;
	while (i + vlength < 28) { // Vlength 이후의 값만 읽겠다

		++i; // [CODE][VALUE] 형식이기 때문에 현재 i 는 [Value] 혹은 [Vlength]에 있다
		
		if (buffPayload[3 + i + vlength] == 0x04) { // 만약에 vlength이후의 값이 0x04라면 Attention
			printf("Attention : %d\n", buffPayload[4 + i + vlength]);
			if (buffPayload[4 + i + vlength] > 60) pushSpace();
		}
		if (buffPayload[3 + i + vlength] == 0x05) { // 만약에 vlength이후의 값이 0x05라면 meditation
			printf("Meditation : %d\n", buffPayload[4 + i + vlength]);
		}
	}
	printf("\n");
}

void pushSpace() {
	INPUT space = { 0 };
	space.type = INPUT_KEYBOARD;
	space.ki.wVk = VK_LCONTROL;
	SendInput(1, &space, sizeof(INPUT)); // Send KeyDown
	Sleep(50);
	space.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &space, sizeof(INPUT)); // Send KeyUp
}