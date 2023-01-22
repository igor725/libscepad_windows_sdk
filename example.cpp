#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include "libScePad.h"

int main(int argc, char* argv[]) {
	int PadHandle = 0;

	// Initialize ScePad
	if (scePadInit() != SCE_OK)
		return -1;

	PadHandle = scePadOpen(1, 0, 0);

	if (PadHandle <= 0) {
		return -2;
	}

	scePadSetParticularMode(true);
	fwrite("\x1B[2J", 4, 1, stdout);

	s_SceEffectData test {};

	while (1) {
		s_ScePadData data;
		memset(&data, 0xFE, sizeof(data));
		fwrite("\x1B[H", 4, 1, stdout);

		if (scePadReadState(PadHandle, &data) == SCE_OK) {
			printf("Buttons: %08X\n", data.Buttons);

			printf("Left stick: %02X %02X\n", data.Sticks.Left.X, data.Sticks.Left.Y);
			printf("Right stick: %02X %02X\n", data.Sticks.Right.X, data.Sticks.Right.Y);

			int state[2]; scePadGetTriggerEffectState(PadHandle, state);
			printf("L2: %02X (%02d), R2: %02X (%02d)\n", data.Triggers.Left, state[0], data.Triggers.Right, state[1]);

			printf("%d touches: %05d, %05d, %05d, %05d, %03d, %03d\n",
				data.Touch.Count,
				data.Touch.Points[0].X, data.Touch.Points[0].Y,
				data.Touch.Points[1].X, data.Touch.Points[1].Y,
				data.Touch.Points[0].ID, data.Touch.Points[1].ID
			);

			printf("Gyroscope data: ");
			for (int i = 0; i < sizeof(data.Gyroscope) / sizeof(data.Gyroscope[0]); i++)
				printf("%02.3f ", data.Gyroscope[i]);

			printf("\nUnknown state fields:\n");
			for (auto udata = (char *)&data.__Unknown; udata < (char *)&data + sizeof(data); udata += 4)
				printf("\t%08d: %02X %02X %02X %02X\n",
					(int)((intptr_t)udata - (intptr_t)&data),
					udata[0] & 0xFF, udata[1] & 0xFF,
					udata[2] & 0xFF, udata[3] & 0xFF
				);

			if (data.Buttons & SCE_BUTTON_L1) {
				test.Left()->Vibration(3, 4, 8);
				test.Right()->Weapon(4, 8, 8);
			} else if (data.Buttons & SCE_BUTTON_R1) {
				test.Left()->Disable();
				test.Right()->Disable();
			}

			if (data.Buttons & (SCE_BUTTON_L1 | SCE_BUTTON_R1))
				scePadSetTriggerEffect(PadHandle, &test);

			if (data.Buttons & SCE_BUTTON_OPTIONS)
				break;
		}

		Sleep(200);
	}

	scePadClose(PadHandle);

	return 0;
}
