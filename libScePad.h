/*

libScePad

Copyright by Sony Computer Entertainment
Header by github/MasonLeeBack
Updated by github/igor725

*/

#ifndef _LIBSCEPAD_H_
#define _LIBSCEPAD_H_

#include <stdint.h>

#ifndef __cplusplus
#	define bool char
#	ifndef true
#		define true 1
#		define false 0
#	endif
#else
#	ifndef LIBSCEPAD_NO_THROW
#		include <cassert>
#	endif
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

// Audio paths for scePadSetAudioOutPath
#define SCE_PAD_AUDIO_OUT_PATH_STEREO_HEADSET 0
#define SCE_PAD_AUDIO_OUT_PATH_MONO_HEADSET 1
#define SCE_PAD_AUDIO_OUT_PATH_MONO_HEADSET_SPEAKER 2
#define SCE_PAD_AUDIO_OUT_PATH_SPEAKER 3
#define SCE_PAD_AUDIO_OUT_PATH_OFF 4

// Device classes scePadGetControllerInformation
#define SCE_PAD_DEVICE_CLASS_INVALID -1
#define SCE_PAD_DEVICE_CLASS_STANDARD 0
#define SCE_PAD_DEVICE_CLASS_GUITAR 1
#define SCE_PAD_DEVICE_CLASS_DRUM 2
#define SCE_PAD_DEVICE_CLASS_DJ_TURNTABLE 3
#define SCE_PAD_DEVICE_CLASS_DANCEMAT 4
#define SCE_PAD_DEVICE_CLASS_NAVIGATION 5
#define SCE_PAD_DEVICE_CLASS_STEERING_WHEEL 6
#define SCE_PAD_DEVICE_CLASS_STICK 7
#define SCE_PAD_DEVICE_CLASS_FLIGHT_STICK 8
#define SCE_PAD_DEVICE_CLASS_GUN 9

typedef struct {
	char __Unknown[8192];
} s_SceContainerInfo;

typedef struct {
	float PixelDensity;
	uint16_t Res_x, Res_y;
} s_ScePadTouchInfo;

typedef struct {
	uint8_t DeadZoneLeft, DeadZoneRight;
} s_ScePadStickInfo;

typedef struct {
	s_ScePadTouchInfo TouchInfo;
	s_ScePadStickInfo StickInfo;
	uint8_t ConnectionType;
	uint8_t ConnectedCount;
	bool Connected;
	uint32_t DeviceClass;
	uint8_t Reserved[8];
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
		for (char *c = Data.__Dummy; c < &Data.__Dummy[sizeof(Data.__Dummy)]; c++)
			*c = '\0';
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

typedef struct {
	float X, Y, Z, W;
} s_SceQuaternion;

typedef struct {
	float X, Y, Z;
} s_SceVector;

typedef struct {
	uint32_t ExtensionUnitId;
	uint8_t Reserve;
	uint8_t DataLen;
	uint8_t Data[10];
} s_ScePadExtUnitData;

// sizeof() == 120 bytes
typedef struct {
	uint32_t Buttons;
	s_SceSticks Sticks;
	s_SceTriggers Triggers;
	s_SceQuaternion Orientation;
	s_SceVector Acceleration;
	s_SceVector AngularVelocity;
	s_SceTouchData Touch;

	bool Connected;
	uint64_t Timestamp;
	s_ScePadExtUnitData ExtUnitData;
	uint8_t ConnectionCount;
	uint8_t Reserved[2];
	uint8_t DeviceUniqueDataLen;
	uint8_t DeviceUniqueData[12];
} s_ScePadData;

typedef struct {
	uint8_t LargeMotor;
	uint8_t SmallMotor;
} s_ScePadVibration;


typedef struct {
	uint8_t SpeakerVol;
	uint8_t JackVol;
	uint8_t Reserved;
	uint8_t MicGain;
} s_ScePadVolumeGain;

#ifdef __cplusplus
extern "C" {
#endif

int  scePadClose(int handle);
int  scePadGetContainerIdInformation(int handle, s_SceContainerInfo *info);
int  scePadGetControllerInformation(int handle, s_ScePadInfo *info);
int  scePadGetControllerType(int handle, int *type);
int  scePadGetHandle(int userID, int, int /*unused ints, must be 0*/);
int  scePadGetJackState(int handle, int state[4]);
bool scePadGetParticularMode(void);
int  scePadGetTriggerEffectState(int handle, int state[2]);
int  scePadInit(void);
int  scePadIsControllerUpdateRequired(int handle);
int  scePadIsSupportedAudioFunction(int handle);
int  scePadOpen(int userID, int, int /*unused ints, must be 0*/);
int  scePadRead(int handle, s_ScePadData data[/*count*/], int count);
int  scePadReadState(int handle, s_ScePadData *data);
int  scePadResetLightBar(int handle);
int  scePadResetOrientation(int handle);
int  scePadSetAngularVelocityDeadbandState(int handle, bool state);
int  scePadSetAudioOutPath(int handle, int path);
int  scePadSetLightBar(int handle, s_SceLightBar* lightdata);
int  scePadSetMotionSensorState(int handle, bool state);
int  scePadSetParticularMode(bool mode);
int  scePadSetTiltCorrectionState(int handle, bool state);
int  scePadSetTriggerEffect(int handle, s_SceEffectData *data);
int  scePadSetVibration(int handle, s_ScePadVibration *vibe);
int  scePadSetVibrationMode(int handle, int mode /*Must be set to 2 for some reason*/);
int  scePadSetVolumeGain(int handle, s_ScePadVolumeGain *vg);

#ifdef __cplusplus
}
#endif

#endif // _LIBSCEPAD_H_
