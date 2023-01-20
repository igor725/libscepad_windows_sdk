#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include "libscepad.h"

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

	while (1) {
		s_ScePadData data;
		memset(&data, 0xFE, sizeof(data));
		fwrite("\x1B[H", 4, 1, stdout);

		if (scePadReadState(PadHandle, &data) == SCE_OK) {
			// Mask for buttons
			printf("Buttons: %08X\n", data.Buttons);

			// Sticks
			printf("Left stick: %02X %02X\n", data.Sticks.Left.X, data.Sticks.Left.Y);
			printf("Right stick: %02X %02X\n", data.Sticks.Right.X, data.Sticks.Right.Y);

			printf("L2: %02X, R2: %02X\n", data.Triggers.Left, data.Triggers.Right);

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

			if (data.Buttons & SCE_BUTTON_OPTIONS)
				break;
		}

		Sleep(200);
	}

	scePadClose(PadHandle);

	return 0;
}
