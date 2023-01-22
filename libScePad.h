/*

libScePad

Copyright by Sony Computer Entertainment
Header by github/MasonLeeBack
Updated by github/igor725

*/

#ifndef _LIBSCEPAD_H_
#define _LIBSCEPAD_H_

#include <stdint.h>

#ifndef LIBSCEPAD_NO_THROW
#	include <cassert>
#endif

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

// Trigger effects (only for DualSense)
#define SCE_TRSELECT_NONE      0x00
#define SCE_TRSELECT_LEFT      0x01
#define SCE_TRSELECT_RIGHT     0x02
#define SCE_TRSELECT_BOTH      (SCE_TRSELECT_LEFT | SCE_TRSELECT_RIGHT)

#define SCE_TREFFECT_DISABLED  0x00
#define SCE_TREFFECT_FEEDBACK  0x01
#define SCE_TREFFECT_WEAPON    0x02
#define SCE_TREFFECT_VIBRATION 0x03

typedef struct {
	char __Unknown[8192];
} s_SceContainerInfo;

typedef struct {
	float __Unknown_Float; // Always 44.86 for DualSense idk what the hell it is
	uint16_t __Unknown_Short[2]; // Touchpad resolution X, Y??
	// The next two bytes always set to 13 for DualSense
	uint8_t __Unknown_Byte0;
	uint8_t __Unknown_Byte1;
	char __May_Be_Offset;
	char __May_Be_Bool1;
	char __May_Be_Bool2;
	// These fields are always set to zero
	long long __Unknown_Long_Long;
	long __Unknown_Long;
} s_ScePadInfo;

typedef struct _SceTriggerEffect {
	uint32_t Type;
	char __Offset[4];
	union {
		struct {
			uint8_t Position;
			uint8_t Strength;
		} Feedback;
		struct {
			uint8_t StartPosition;
			uint8_t EndPosition;
			uint8_t Strength;
		} Weapon;
		struct {
			uint8_t Type;
			uint8_t Position;
			uint8_t Strength;
		} Vibration;
		char __Dummy[48];
	} Data;

#ifdef __cplusplus
	_SceTriggerEffect() { Disable(); }

	inline void Feedback(uint8_t pos, uint8_t str) {
#		ifndef LIBSCE_NO_THROW
			assert(pos < 10 && str < 9);
#		endif
		Type = SCE_TREFFECT_FEEDBACK;
		Data.Feedback.Position = pos;
		Data.Feedback.Strength = str;
	}

	inline void Vibration(uint8_t type, uint8_t pos, uint8_t str) {
#		ifndef LIBSCE_NO_THROW
			assert(type < 10 && pos < 10 && str < 9);
#		endif
		Type = SCE_TREFFECT_VIBRATION;
		Data.Vibration.Type = type;
		Data.Vibration.Position = pos;
		Data.Vibration.Strength = str;
	}

	inline void Weapon(uint8_t start, uint8_t end, uint8_t str) {
#		ifndef LIBSCE_NO_THROW
			assert(start < 10 && end > start && str < 9);
#		endif
		Type = SCE_TREFFECT_WEAPON;
		Data.Weapon.StartPosition = start;
		Data.Weapon.EndPosition = end;
		Data.Weapon.Strength = str;
	}

	inline void Disable(void) {
		Type = SCE_TREFFECT_DISABLED;
		memset(Data.__Dummy, 0, sizeof(Data.__Dummy));
	}
#endif
} s_SceTriggerEffect;

typedef struct _SceEffectData {
	uint32_t Select;
	char __Offset[4];
	s_SceTriggerEffect Triggers[2];

#ifdef __cplusplus
	_SceEffectData() : Select(0) {}

	inline s_SceTriggerEffect *Left(void) {
		Select |= SCE_TRSELECT_LEFT;
		return &Triggers[0];
	}

	inline s_SceTriggerEffect *Right(void) {
		Select |= SCE_TRSELECT_RIGHT;
		return &Triggers[1];
	}
#endif
} s_SceEffectData;

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

// sizeof() == 120 bytes
typedef struct {
	uint32_t Buttons;
	s_SceSticks Sticks;
	s_SceTriggers Triggers;
	float Gyroscope[10]; // Eh?
	s_SceTouchData Touch;

// TODO: Things to be figured out later
	char __Unknown[44];
} s_ScePadData;

#ifdef __cplusplus
extern "C" {
#else
#define bool BOOLEAN
#endif

int scePadClose(int handle);
int scePadGetContainerIdInformation(int handle, s_SceContainerInfo *info);
int scePadGetControllerInformation(int handle, s_ScePadInfo *info);
int scePadGetControllerType(int handle, int *type);
int scePadGetHandle(int userID, int, int /*unused ints, must be 0*/);
int scePadGetJackState(int handle, int state[4]);
bool scePadGetParticularMode(void);
int scePadGetTriggerEffectState(int handle, int state[2]);
int scePadInit(void);
int scePadIsSupportedAudioFunction(int handle);
int scePadOpen(int userID, int, int /*unused ints, must be 0*/);
int scePadRead(int handle, void* data, int count);
int scePadReadState(int handle, s_ScePadData* data);
int scePadResetLightBar(int handle);
int scePadResetOrientation(int handle);
int scePadSetAngularVelocityDeadbandState(int handle, bool state);
int scePadSetAudioOutPath(int handle);
int scePadSetLightBar(int handle, s_SceLightBar* lightdata);
int scePadSetMotionSensorState(int handle, bool state);
int scePadSetParticularMode(bool);
int scePadSetTiltCorrectionState(int handle, bool state);
int scePadSetTriggerEffect(int handle, s_SceEffectData *data);
int scePadSetVibration(int handle, char values[2]);
int scePadSetVibrationMode(int handle, int mode /*Must be set to 2 for some reason*/);
int scePadSetVolumeGain(int handle, void *);

#ifdef __cplusplus
}
#endif

#endif // _LIBSCEPAD_H_
