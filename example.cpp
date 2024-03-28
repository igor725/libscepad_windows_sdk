#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <cmath>
#include "libScePad.h"

int main(int argc, char* argv[]) {
	int PadHandle = 0;

	if (scePadInit() != SCE_OK)
		return -1;

	PadHandle = scePadOpen(1, 0, 0);

	if (PadHandle <= 0) {
		return -2;
	}

	scePadSetParticularMode(true);
	fwrite("\x1B[2J", 4, 1, stdout);

	s_SceEffectData test {};
	s_SceLightBar color = {0, 0, 0};

	while (true) {
		s_ScePadData data;
		fwrite("\x1B[H", 4, 1, stdout);

		if (scePadReadState(PadHandle, &data) == SCE_OK) {
			printf("Buttons: %08X\n", data.Buttons);

			printf("Left stick: %02X %02X\n", data.Sticks.Left.X, data.Sticks.Left.Y);
			printf("Right stick: %02X %02X\n", data.Sticks.Right.X, data.Sticks.Right.Y);

			int state[2]; scePadGetTriggerEffectState(PadHandle, state);
			printf("L2: %02X (%02d), R2: %02X (%02d)\n", data.Triggers.Left, state[0], data.Triggers.Right, state[1]);

			printf("%d touches: %d, %d, %d, %d, %d, %d\x1B[K\n",
				data.Touch.Count,
				data.Touch.Points[0].X, data.Touch.Points[0].Y,
				data.Touch.Points[1].X, data.Touch.Points[1].Y,
				data.Touch.Points[0].ID, data.Touch.Points[1].ID
			);

			printf("Gyroscope data: %.3f, %.3f, %.3f, %.3f\x1B[K\n",
				data.Orientation.X, data.Orientation.Y,
				data.Orientation.Z, data.Orientation.W
			);

			printf("Timestamp: %llu\x1B[K\n", data.Timestamp);

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

			static const float tau = acosf(-1.0f) * 2.f;
			static float timer = 0.0f;
			color.R = sqrtf(sinf(timer * tau) * 0.5f + 0.5f) * 0xFF;
			color.G = sqrtf(sinf((timer + 0.666f) * tau) * 0.5f + 0.5f) * 0xFF;
			color.B = sqrtf(sinf((timer + 0.333f) * tau) * 0.5f + 0.5f) * 0xFF;
			scePadSetLightBar(PadHandle, &color);
			timer += 0.032f;
		}

		Sleep(200);
	}

	scePadResetLightBar(PadHandle);
	scePadClose(PadHandle);

	return 0;
}
