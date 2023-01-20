/*

libScePad

Copyright by Sony Computer Entertainment
Header by github/MasonLeeBack
Updated by github/igor725

*/

#ifndef _LIBSCEPAD_H_
#define _LIBSCEPAD_H_

#include <stdint.h>

#if !defined(_WIN32) || !defined(_WIN64)
#	error This library is only compatible with Windows 64-bit systems.
#endif

// Error codes
#define SCE_ERROR(X) (0x80920000 | X)

#define SCE_OK                        0x00000000
#define SCE_ERROR_INVALID_ARGUMENT    SCE_ERROR(0x01)
#define SCE_ERROR_UNKNOWN_1           SCE_ERROR(0x02)
#define SCE_ERROR_INVALID_PAD_HANDLE  SCE_ERROR(0x03)
#define SCE_ERROR_UNKNOWN_2           SCE_ERROR(0x04)
#define SCE_ERROR_NOT_INITED          SCE_ERROR(0x05)
#define SCE_ERROR_UNKNOWN_3           SCE_ERROR(0x06)
#define SCE_ERROR_UNKNOWN_4           SCE_ERROR(0x07)
#define SCE_ERROR_UNKNOWN_5           SCE_ERROR(0x08)
#define SCE_ERROR_THREAD_IS_NOT_READY SCE_ERROR(0xFF)

// Button mask
#define SCE_BUTTON_CROSS    0x00004000
#define SCE_BUTTON_CIRCLE   0x00002000
#define SCE_BUTTON_TRIANGLE 0x00001000
#define SCE_BUTTON_SQUARE   0x00008000

#define SCE_BUTTON_L1       0x00000400
#define SCE_BUTTON_L2       0x00000100
#define SCE_BUTTON_R1       0x00000800
#define SCE_BUTTON_R2       0x00000200

#define SCE_BUTTON_L3       0x00000002
#define SCE_BUTTON_R3       0x00000004

#define SCE_BUTTON_N_DPAD   0x00000010
#define SCE_BUTTON_S_DPAD   0x00000040
#define SCE_BUTTON_E_DPAD   0x00000020
#define SCE_BUTTON_W_DPAD   0x00000080

#define SCE_BUTTON_OPTIONS  0x00000008

#define SCE_BUTTON_TOUCH    0x00100000
#define SCE_BUTTON_MUTE     0x00200000

// Call sceSetParticularMode(true) to use these
#define SCE_BUTTON_SHARE    0x00000001
#define SCE_BUTTON_PSBTN    0x00010000

typedef struct {
	uint8_t X;
	uint8_t Y;
} s_SceStickData;

typedef struct {
	uint16_t X;
	uint16_t Y;
	uint16_t ID;
	uint16_t __Unused;
} s_SceTouchPoint;

typedef struct {
	uint8_t Count;
	char __Pad[7];
	s_SceTouchPoint Points[2];
} s_SceTouchData;

typedef struct {
	uint8_t R;
	uint8_t G;
	uint8_t B;
} s_SceLightBar;

typedef struct {
	s_SceStickData Left;
	s_SceStickData Right;
} s_SceSticks;

typedef struct {
	uint8_t Left;
	uint8_t Right;
} s_SceTriggers;

// TODO: Things to be figured out later
// 120 bytes
typedef struct {
	uint32_t Buttons;
	s_SceSticks Sticks;
	s_SceTriggers Triggers;
	float Gyroscope[10]; // Eh?
	s_SceTouchData Touch;
	char __Unknown[44];
} s_ScePadData;

#ifdef __cplusplus
extern "C" {
#endif

int scePadClose(int handle);
int scePadGetContainerIdInformation(int handle, void *);
int scePadGetControllerInformation(int handle, char info[28]);
int scePadGetControllerType(int handle, int *type);
int scePadGetHandle(int userID, int, int /*unused ints, must be 0*/);
int scePadGetJackState(int handle, int state[4]);
bool scePadGetParticularMode(void);
int scePadGetTriggerEffectState(void);
int scePadInit(void);
int scePadIsSupportedAudioFunction(int handle);
int scePadOpen(int userID, int, int /*unused ints, must be 0*/);
int scePadRead(int handle, void* data, int count);
int scePadReadState(int handle, s_ScePadData* data);
int scePadResetLightBar(int handle);
int scePadResetOrientation(int handle);
int scePadSetAngularVelocityDeadbandState(int handle, char state);
int scePadSetAudioOutPath(int handle);
int scePadSetLightBar(int handle, s_SceLightBar* lightdata);
int scePadSetMotionSensorState(int handle, char state);
int scePadSetParticularMode(bool);
int scePadSetTiltCorrectionState(int handle, char state);
int scePadSetTriggerEffect(int handle, void *effects);
int scePadSetVibration(int handle, char values[2]);
int scePadSetVibrationMode(int handle, int mode /*Must be set to 2 for some reason*/);
int scePadSetVolumeGain(int handle, void *);

#ifdef __cplusplus
}
#endif

#endif // _LIBSCEPAD_H_
