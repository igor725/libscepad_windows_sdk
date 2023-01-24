local ffi = require('ffi')
local lib = ffi.load('libScePad.dll', false)
ffi.cdef[[
	typedef struct {
		char __Unknown[8192];
	} s_SceContainerInfo;
	
	typedef struct {
		float __Unknown_Float; // Always 44.86 (for DualSense at least) idk what the hell it is
		uint16_t Touchpad_Resolution[2];
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
	} s_SceTriggerEffect;

	typedef struct _SceEffectData {
		uint32_t Select;
		char __Offset[4];
		s_SceTriggerEffect Triggers[2];
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

	int  scePadClose(int handle);
	int  scePadGetContainerIdInformation(int handle, s_SceContainerInfo *info);
	int  scePadGetControllerInformation(int handle, s_ScePadInfo *info);
	int  scePadGetControllerType(int handle, int *type);
	int  scePadGetHandle(int userID, int, int /*unused ints, must be 0*/);
	int  scePadGetJackState(int handle, int state[4]);
	bool scePadGetParticularMode(void);
	int  scePadGetTriggerEffectState(int handle, int state[2]);
	int  scePadInit(void);
	int  scePadIsSupportedAudioFunction(int handle);
	int  scePadOpen(int userID, int, int /*unused ints, must be 0*/);
	int  scePadRead(int handle, s_ScePadData data[/*count*/], int count);
	int  scePadReadState(int handle, s_ScePadData *data);
	int  scePadResetLightBar(int handle);
	int  scePadResetOrientation(int handle);
	int  scePadSetAngularVelocityDeadbandState(int handle, bool state);
	int  scePadSetAudioOutPath(int handle, int);
	int  scePadSetLightBar(int handle, s_SceLightBar* lightdata);
	int  scePadSetMotionSensorState(int handle, bool state);
	int  scePadSetParticularMode(bool);
	int  scePadSetTiltCorrectionState(int handle, bool state);
	int  scePadSetTriggerEffect(int handle, s_SceEffectData *data);
	int  scePadSetVibration(int handle, char values[2]);
	int  scePadSetVibrationMode(int handle, int mode /*Must be set to 2 for some reason*/);
	int  scePadSetVolumeGain(int handle, void *);
]]

assert(lib.scePadInit() == 0, 'Library initialization failed')
local s_ScePadDataA = ffi.typeof('s_ScePadData[?]')
local s_ScePadData = ffi.typeof('s_ScePadData')
local function errorToString(code)

	return 'Unknown error'
end

local _L = {}
local pad = {
	__tostring = function(self)
		return ('Pad[%d]'):format(self[1])
	end
}
pad.__index = pad

function pad:read(state)
	state = state or ffi.new(s_ScePadData, 1)
	local ret = lib.scePadReadState(self[1], state)
	if ret == 0 then return state end
	return nil, errorToString(ret)
end

function pad:close()
	lib.scePadClose(self[1])
end

function _L.open(userID)
	local ret = lib.scePadOpen(userID, 0, 0)
	if ret <= 0 then
		return nil, errorToString(ret)
	end

	return setmetatable({ret}, pad)
end

function _L.newPadState(count)
	if type(count) ~= 'number' or count < 2 then
		return ffi.new(s_ScePadData)
	end

	return ffi.new(s_ScePadDataA, count)
end

return _L
