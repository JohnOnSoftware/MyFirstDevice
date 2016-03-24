#ifndef __ORDEVICE_TEMPLATE_HARDWARE_H__
#define __ORDEVICE_TEMPLATE_HARDWARE_H__


/**	\file	ordevice_template_hardware.h
*	Declaration of simple output hardware.
*	Declaration of the ORDevice_Template_Hardware class, representing
*	a light console being driven by FiLMBOX.
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Array size defines
#define	MAX_BUFFER_SIZE		2048
#define MAX_DATA_SIZE		12
#define	MAX_INFO_SIZE		4
#define MAX_REQUEST_SIZE	4

//--- Communication protocol defines
#define	BYTE_HEADER			0x3c
#define	BYTE_INFO_PACKET	0x49
#define BYTE_DATA_PACKET	0x44
#define BYTE_STREAM_START	0x41
#define BYTE_STREAM_STOP	0x42
#define BYTE_TRAILER		0x3e

//! Read state enum.
enum ORReadState
{
	eORReadStateHeader,
	eORReadStateType,
	eORReadStateData,
	eORReadStateCRC,
	eORReadStateTrailer
};

class ORDevice_Template;

//! Device hardware template
class ORDevice_Template_Hardware
{
public:
	ORDevice_Template_Hardware();							//!< Constructor.
	~ORDevice_Template_Hardware();							//!< Destructor.

	void		SetParent(FBDevice* pParent);

	//--- Communications
	bool		Open();										//!< Open the device.
	bool		Close();									//!< Close the device.
	int			FetchData			();						//!< Fetch a data packet.
	bool		PollData			();						//!< Poll for new data.

	//--- Attribute management
	void		SetStreaming		(bool pStreaming)		{	mStreaming = pStreaming;		}
	bool		GetStreaming		()						{	return mStreaming;				}
	void		SetCommunicationType(FBCommType pType);
	int			GetCommunicationType();
	void		SetNetworkAddress	(const char* pIPAddress){	mNetworkAddress =  pIPAddress;	}
	const char* GetNetworkAddress	()						{	return mNetworkAddress;	}
	void		SetNetworkPort		(int pPort)				{ 	mNetworkPort = pPort;			}
	int			GetNetworkPort		()						{ 	return mNetworkPort;			}
	void		SetSerialPort		(int pPort)				{ 	mSerialPort = pPort;			}
	int			GetSerialPort		()						{ 	return mSerialPort;				}
	void		SetSerialBaudRate	(int pRate)				{	mSerialBaudRate = pRate;		}
	int			GetSerialBaudRate	()						{ 	return mSerialBaudRate;			}

	//--- Device interaction
	bool		GetSetupInfo		();
	bool		StartStream			();
	bool		StopStream			();

	//--- Hardware abstraction of device positional information
	void		GetPosition			( double* pPos );
	void		GetRotation			( double* pRot );

	//--- Abstraction of communication type read/write functions
	bool		WriteData			( unsigned char* pBuffer, int pSize, int* pNumberOfBytesWritten=NULL	);
	bool		ReadData			( unsigned char* pBuffer, int pSize, int* pNumberOfBytesRead=NULL		);
	void		EmptyBuffer			();

	//--- Simulator functions
	void		SetFrequency		( double pFrequency )	{ mFrequency = pFrequency;		}
	double		GetFrequency		()						{ return mFrequency;			}
	void		SetAmplitude		( double pAmplitude )	{ mAmplitude = pAmplitude;		}
	double		GetAmplitude		()						{ return mAmplitude;			}

private:
	//--- Utility members
	FBSystem		mSystem;								//!< System interface.
	FBDevice*		mParent;

	//--- Data extraction members
	ORReadState		mReadState;								//!< Current read state.
	unsigned char	mBuffer	[ MAX_BUFFER_SIZE	];			//!< Read buffer.
	unsigned char	mData	[ MAX_DATA_SIZE		];			//!< Data packet buffer.
	unsigned char	mRequest[ MAX_REQUEST_SIZE	];			//!< Request packet buffer.
	int				mDataCount;								//!< Count for read into data packet buffer.

	//--- Communications members
	FBCommPort		mCommPort;								//!< CommPort interface.
	FBTCPIP			mTCPIP;									//!< TCPIP interface.
	int				mNetworkSocket;							//!< Network socket.
	FBString		mNetworkAddress;						//!< Network address.
	int				mNetworkPort;							//!< Network port number.
	int				mSerialPort;							//!< Serial port number.
	int				mSerialBaudRate;						//!< Serial baud rate.
	bool			mStreaming;								//!< Is device in streaming mode?

	//--- Device channel status
	double			mPosition[3];							//!< Position of input from hardware.
	double			mRotation[3];							//!< Rotation of input from hardware.

	//--- Simulator values
	double			mFrequency;								//!< Simulator frequency.
	double			mAmplitude;								//!< Simulator amplitude.

};

#endif /* __ORDEVICE_TEMPLATE_HARDWARE_H__ */
