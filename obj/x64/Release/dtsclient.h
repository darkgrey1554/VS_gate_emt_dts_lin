//
// ������ DTS
//
// �������� ����� �.�.

#ifndef __DTSCLIENT_H__
#define __DTSCLIENT_H__

#include "values.h"
#include "errkey.h"

// ��� ������� ������������ ��� ��������� �������� ������ � ������������ ��������� ������
typedef int (* RcvFunc)( void *,Value &,int32_t );
typedef int (* ErrFunc)( void *,Value &,int32_t );
//
// ������ DTS
//
class DTSClient : public ErrKey
	{
private:
	static void * pthreadFunc( void * );

public:
	// �������� ����� ���������� ������� DTS
	int getChnlNum();

	// �������� ���������� ������
	int isActive( int32_t _chnlId = 0 );

	// ��������� ������� � ����� DTS
	int put( Value *, int32_t _chnlId = 0 );
	inline int put( Value & _value, int32_t _chnlId = 0 ){ return put( &_value,_chnlId ); };

	// �������� ��������� �������� ��������
	int getRcvValue( Value * ,int32_t _chnlId = 0 );
	inline int getRcvValue( Value & _value,int32_t _chnlId = 0 ){ return getRcvValue( &_value,_chnlId ); };

	// �������� ��������� ���������� ��������
	int getSndValue( Value *,int32_t _chnlId = 0 );
	inline int getSndValue( Value & _value,int32_t _chnlId = 0 ){ return getSndValue( &_value,_chnlId ); };

	// ���������� �������
 	int stopReceive( int32_t _chnlId = 0 );
 	int startReceive( int32_t _chnlId = 0 );

 	// ���������� ���������
 	int stopSend( int32_t _chnlId = 0 );
 	int startSend( int32_t _chnlId = 0 );

	// �������� ���������� �������� ��� ������ �� �����
	uint32_t getRcvAnaNum( int32_t _chnlId = 0 );
	uint32_t getRcvBinNum( int32_t _chnlId = 0 );
	uint32_t getRcvIntNum( int32_t _chnlId = 0 );

	// �������� ���������� �������� ��� �������� �� �����
	uint32_t getSndAnaNum( int32_t _chnlId = 0 );
	uint32_t getSndBinNum( int32_t _chnlId = 0 );
	uint32_t getSndIntNum( int32_t _chnlId = 0 );

	// ���������� ����������� ������ � ������
	int transOn( int32_t _chnlId = 0 );
	int transOff( int32_t _chnlId = 0 );

	DTSClient( RcvFunc _rcvFunc = NULL,void * _args_1 = NULL,ErrFunc _errFunc = NULL,void * _args_2 = NULL,uint16_t _port = 49000 );
	~DTSClient();
	};
#endif
