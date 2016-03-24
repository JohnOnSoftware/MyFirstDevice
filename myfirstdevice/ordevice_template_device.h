#ifndef __ORDEVICE_TEMPLATE_DEVICE_H__
#define __ORDEVICE_TEMPLATE_DEVICE_H__


//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Class declaration
#include "ordevice_template_hardware.h"

//--- Registration defines
#define ORDEVICETEMPLATE__CLASSNAME		ORDevice_Template
#define ORDEVICETEMPLATE__CLASSSTR		"ORDevice_Template"

//TBD uncomment the class declaration
class ORDevice_Template : public FBDevice
{
	//FBDeviceDeclare( ORDevice_Template, FBDevice );

public:
	virtual bool FBCreate();	
	virtual void FBDestroy();

	//--- The following will be called by the real-time engine.
	virtual bool AnimationNodeNotify	(	FBAnimationNode* pAnimationNode,	FBEvaluateInfo* pEvaluateInfo			);	//!< Real-time evaluation for node.
	virtual void DeviceIONotify			(	kDeviceIOs  pAction,				FBDeviceNotifyInfo &pDeviceNotifyInfo	);	//!< Notification of/for Device IO.
    virtual bool DeviceEvaluationNotify	(	kTransportMode pMode,				FBEvaluateInfo* pEvaluateInfo			);	//!< Evaluation the device (write to hardware).

    
    virtual bool DeviceOperation		(	kDeviceOperations pOperation												);	//!< Operate device.

	//--- FBX Load/Save.
	virtual bool FbxStore	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat	);	//!< Store in FBX file.
	virtual bool FbxRetrieve( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat	);	//!< Retrieve from FBX file.

	//--- Initialisation/Shutdown
	bool		Init();			
	bool		Start();		
	bool		Reset();		
	bool		Stop();			
	bool		Done();			

	//--- Recording
	void		DeviceRecordFrame( FBDeviceNotifyInfo &pDeviceNotifyInfo );

	//--- Aggregation of hardware parameters
	void		SetCommunicationType( FBCommType pType)		{ mHardware.SetCommunicationType( pType );		}
	int			GetCommunicationType()						{ return mHardware.GetCommunicationType();		}

	void		SetNetworkAddress	(const char* pIPAddress){ mHardware.SetNetworkAddress(pIPAddress);		}
	const char*	GetNetworkAddress	()						{ return mHardware.GetNetworkAddress();			}
	void		SetNetworkPort		(int pPort)				{ mHardware.SetNetworkPort(pPort);				}
	int			GetNetworkPort		()						{ return mHardware.GetNetworkPort();			}
	void		SetSerialPort		(int pPort)				{ mHardware.SetSerialPort(pPort);				}
	int			GetSerialPort		()						{ return mHardware.GetSerialPort();				}
	void		SetSerialBaudRate	(int pRate)				{ mHardware.SetSerialBaudRate(pRate);			}
	int			GetSerialBaudRate	()						{ return mHardware.GetSerialBaudRate();			}
	void		SetStreaming		(bool pStreaming)		{ mHardware.SetStreaming( pStreaming );			}
	bool		GetStreaming		()						{ return mHardware.GetStreaming();				}

	//--- Aggregation of simulator parameters
	void		SetFrequency		( double pFrequency )	{ mHardware.SetFrequency( pFrequency );			}
	double		GetFrequency		()						{ return mHardware.GetFrequency();				}
	void		SetAmplitude		( double pAmplitude )	{ mHardware.SetAmplitude( pAmplitude );			}
	double		GetAmplitude		()						{ return mHardware.GetAmplitude();				}

	bool		GetSetCandidate		()						{ return mSetCandidate;							}
	void		SetSetCandidate		( bool pState )			{ mSetCandidate = pState;						}
	double		GetSamplingRate		()						{ return mSamplingRate;							}
	void		SetSamplingRate		( double pRate )		{ mSamplingRate = pRate;						}
	FBDeviceSamplingMode	GetSamplingType()								{ return mSamplingType;			}
	void					SetSamplingType( FBDeviceSamplingMode pType )	{ mSamplingType = pType;		}

	void		SetCandidates		();

public:
	FBModelTemplate*				mTemplateRoot;			//!< Root model template.
	FBModelTemplate*				mTemplateMarker;		//!< Marker model template.
	FBAnimationNode*				mNodeMarker_InT;		//!< Light input animation node (translation).
	FBAnimationNode*				mNodeMarker_InR;		//!< Light input animation node (rotation).

private:
	bool							mSetCandidate;			//!< Are we setting the candidate or writing to the connector?
	double							mSamplingRate;			//!< Device sampling rate.
	FBDeviceSamplingMode			mSamplingType;			//!< Device sampling type.
	ORDevice_Template_Hardware		mHardware;				//!< Hardware member.
	FBPlayerControl					mPlayerControl;			//!< In order to query the play state for recording.
};

#endif
