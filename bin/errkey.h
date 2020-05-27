#ifndef __ERRKEY_H__
#define __ERRKEY_H__

#include <limits.h>
#include <stdint.h>

// ������������ ������ ������ ��������� �� ������
#define ERR_STR_LEN		256

enum ErrFlag
	{
	Ok = 0,
	NotInit = SHRT_MIN,
	BadInit,
	AlreadyInUse,
	UseBeforeInit,
	NewerUsed,
	
	//������ ����������
	BlockBySystem,
	BlockByExtern,
	BlockByUser,
	
	// ������ "������"
	BadCommand,
	BadArg,
	BadSize,
	BadType,
	BadInput,
	BadOutput,
	BadString,
	BadOffs,
	BadHardware,
	BadCfg,
	BadVersion,
	
	// ������ "��� ���������"
	Overflow,
	Underflow,
	NotInRange,
	Overtime,
	NoMem,
	
	// ������ "�� ������� ������, ���..."
	NotFind,
	NotAllowed,
	NotReadyBut,
	NotStored,
	NotAccess
	};
//
// ������� ����� ������
//
class ErrKey
	{
private :
	int32_t error;				// ������ ��������� ��������

protected :
	ErrFlag state;			// C�������� �������
	char errstr[ERR_STR_LEN];	// ������ ������ ��������� ��������

	// ��������� ������
	inline void _setError( int16_t _error, uint16_t _moduleId = 0, uint16_t _moduleNum = 0 )
		{
		error = ( ( (int32_t)_moduleId & 0xFF ) << 24 ) + ( ( (int32_t)_moduleNum & 0xFF ) << 16 ) + ( _error & 0xFFFF );
		}
	ErrKey(){ state = BadInit; error = 0; *errstr = 0; }
	virtual ~ErrKey(){}

public :
	// �������� ���������
	inline ErrFlag getState(){ return state; }

	// �������� ������ ������
	inline const char * getErrStr(){ return errstr; }

	// �������� ����������� ��� ������
	inline int16_t getError(){ return (int16_t)error; }
	inline int32_t getModuleId(){ return error >> 24; }
	inline int32_t getFuncNum(){ return ( error >> 16 ) & 0xFF; }

	// ��������� ���������
	inline int operator == ( ErrFlag _state ){ return ( state == _state )? 1 : 0; }
	inline int operator != ( ErrFlag _state ){ return ( state != _state )? 1 : 0; }
	};
#endif
