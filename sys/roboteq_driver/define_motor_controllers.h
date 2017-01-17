#ifndef DEFINE_MOTOR_CONTROLLERS_H_
#define DEFINE_MOTOR_CONTROLLERS_H_


// Control whether the setpoint represents a velocity or position command.
// Velocity is the default, but position may be used for fine-grained movements,
// or active braking on an incline. Stopped is high-z (coasting).
#define MODE_STOPPED -1
#define MODE_VELOCITY 0
#define MODE_POSITION 1

#define FAULT_OVERHEAT 1
#define FAULT_OVERVOLTAGE 2
#define FAULT_UNDERVOLTAGE 4
#define FAULT_SHORT_CIRCUIT 8
#define FAULT_EMERGENCY_STOP 16
#define FAULT_SEPEX_EXCITATION_FAULT 32
#define FAULT_MOSFET_FAILURE 64
#define FAULT_STARTUP_CONFIG_FAULT 128

#define STATUS_SERIAL_MODE 1
#define STATUS_PULSE_MODE 2
#define STATUS_ANALOG_MODE 4
#define STATUS_POWER_STAGE_OFF 8
#define STATUS_STALL_DETECTED 16
#define STATUS_AT_LIMIT 32
#define STATUS_MICROBASIC_SCRIPT_RUNNING 128

#define _MOTAMPS 1
#define _MOTCMD 2
#define _MOTPWR 3
#define _ABSPEED 4
#define _ABCNTR 5
#define _VOLTS 6
#define _BATAMPS 7
#define _ANAIN 8
#define _TEMP 9
#define SCRIPT_VER 10
#define _FLTFLAG 11
#define _STFLAG 12

#endif /* DEFINE_MOTOR_CONTROLLERS_H_ */