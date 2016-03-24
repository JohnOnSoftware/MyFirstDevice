//--- Class declaration
#include "ordevice_template_device.h"

//--- Registration defines
#define ORDEVICETEMPLATE__CLASS		ORDEVICETEMPLATE__CLASSNAME
#define ORDEVICETEMPLATE__NAME		ORDEVICETEMPLATE__CLASSSTR
#define ORDEVICETEMPLATE__LABEL		"OR - Device Template"
#define ORDEVICETEMPLATE__DESC		"OR - Device Template Description"
#define ORDEVICETEMPLATE__PREFIX	"DeviceTemplate"

//TBD uncomment the class implementation and registration
/*
FBDeviceImplementation	(	ORDEVICETEMPLATE__CLASS	);
FBRegisterDevice		(	ORDEVICETEMPLATE__NAME,
							ORDEVICETEMPLATE__CLASS,
							ORDEVICETEMPLATE__LABEL,
							ORDEVICETEMPLATE__DESC,
							FB_DEFAULT_SDK_ICON		);	// Icon filename (default=Open Reality icon)
*/

bool ORDevice_Template::FBCreate()
{
	mHardware.SetParent( this );

	// Create animation nodes
	mNodeMarker_InT	= AnimationNodeOutCreate( 0, "Translation",	ANIMATIONNODE_TYPE_LOCAL_TRANSLATION	);
	mNodeMarker_InR	= AnimationNodeOutCreate( 1, "Rotation",	ANIMATIONNODE_TYPE_LOCAL_ROTATION		);

	// Create model templates
	mTemplateRoot	= new FBModelTemplate( ORDEVICETEMPLATE__PREFIX, "Reference",	kFBModelTemplateRoot	);
	mTemplateMarker	= new FBModelTemplate( ORDEVICETEMPLATE__PREFIX, "Marker",		kFBModelTemplateMarker	);

	// Build model template hierarchy
	ModelTemplate.Children.Add(mTemplateRoot);
	mTemplateRoot->Children.Add(mTemplateMarker);

	// Bind the model templates (if applicable) to device's animation nodes
	mTemplateMarker->Bindings.Add( mNodeMarker_InR );
	mTemplateMarker->Bindings.Add( mNodeMarker_InT );

	// Set sampling rate to 60 Hz
	FBTime	lPeriod;
	lPeriod.SetSecondDouble(1.0/60.0);
	SamplingPeriod	= lPeriod;

	CommType = kFBCommTypeSimulator;
	mSetCandidate = false;

	return true;
}

/************************************************
 *	FiLMBOX Destructor.
 ************************************************/
void ORDevice_Template::FBDestroy()
{
}


/************************************************
 *	Device operation.
 ************************************************/
bool ORDevice_Template::DeviceOperation( kDeviceOperations pOperation )
{
    //TBD: Please map command to your operation functions here

	return FBDevice::DeviceOperation( pOperation );
}


/************************************************
 *	Initialization of device.
 ************************************************/
bool ORDevice_Template::Init()
{
    FBProgress	lProgress;

    lProgress.Caption	= "Device Template";
	lProgress.Text		= "Initializing device...";

	return true;
}


/************************************************
 *	Device is put online.
 ************************************************/
bool ORDevice_Template::Start()
{
	FBProgress	lProgress;
	lProgress.Caption	= "Starting up device";

	// Step 1: Open device communications
	lProgress.Text	= "Opening device communications";
	Status			= "Opening device communications";

	if(!mHardware.Open())
	{
		Status = "Could not open device";
		return false;
	}

	// Step 2: Ask hardware to get channel information
	lProgress.Text	= "Device found, getting setup information";
	Status			= "Getting setup information";

	if(!mHardware.GetSetupInfo())
	{
		Status = "Could not get setup information from device.";
		return false;
	}
	else
	{
		HardwareVersionInfo = "Device Template, v1.0";
		Information			= "";
	}

	if( mHardware.GetStreaming() )
	{
		// Step 3: Start streaming data from device
		lProgress.Text	= "Sending START STREAM command";
		Status			= "Starting device streaming";

		if(!mHardware.StartStream())
		{
			Status	= "Could not start stream mode";
			return false;
		}
	}
	Status = "Ok";
	return true;
}


/************************************************
 *	Device is stopped (offline).
 ************************************************/
bool ORDevice_Template::Stop()
{
	FBProgress	lProgress;
	lProgress.Caption	= "Shutting down device";

	if( mHardware.GetStreaming() )
	{
		// Step 1: Stop streaming data
		lProgress.Text	= "Sending STOP STREAM command";
		Status			= "Stopping device streaming";

		if(!mHardware.StopStream())
		{
			Status = "Could not stop streaming";
			return false;
		}
	}

	// Step 1: Stop streaming data
	lProgress.Text	= "Stopping device communications";
	Status			= "Stopping device communications";

	if(!mHardware.Close())
	{
		Status = "Could not close device";
		return false;
	}

	Status			= "?";

    return false;
}


/************************************************
 *	Removal of device.
 ************************************************/
bool ORDevice_Template::Done()
{
	return false;
}


/************************************************
 *	Reset of device.
 ************************************************/
bool ORDevice_Template::Reset()
{
    Stop();
    return Start();
}


/************************************************
 *	Real-Time Engine Evaluation.
 ************************************************/
bool ORDevice_Template::AnimationNodeNotify(FBAnimationNode* pAnimationNode ,FBEvaluateInfo* pEvaluateInfo)
{
    //TBD: Please read data from hardware layer to animation node

    return true;
}


/************************************************
 *	Device Evaluation Notify, for output device only
 ************************************************/
bool ORDevice_Template::DeviceEvaluationNotify( kTransportMode pMode, FBEvaluateInfo* pEvaluateInfo )
{
	return true;
}


/************************************************
 *	Real-Time Synchronous Device IO.
 ************************************************/
void ORDevice_Template::DeviceIONotify( kDeviceIOs  pAction,FBDeviceNotifyInfo &pDeviceNotifyInfo)
{
    //TBD: Please read the data from hardware to hardware abstract layer

}


/************************************************
 *	Record a frame of the device (recording).
 ************************************************/
void ORDevice_Template::DeviceRecordFrame( FBDeviceNotifyInfo &pDeviceNotifyInfo )
{
	FBAnimationNode*	lDataT	= NULL;
	FBAnimationNode*	lDataR = NULL;

	double	lPos[3];
	double	lRot[3];

	FBTime	lTime;

	lTime = pDeviceNotifyInfo.GetLocalTime();

	// Translation information.
	lDataT = mNodeMarker_InT->GetAnimationToRecord();
	if (lDataT)
	{
		if( mPlayerControl.GetTransportMode() == kFBTransportPlay )
		{
			mHardware.GetPosition( lPos );

			switch( SamplingMode )
			{
				case kFBHardwareTimestamp:
				case kFBSoftwareTimestamp:
				{
					lDataT->KeyAdd(lTime, lPos);
				}
				break;

				case kFBHardwareFrequency:
				case kFBAutoFrequency:
				{
					lDataT->KeyAdd(lPos);
				}
				break;
			}
		}
	}

	// Rotation information.
	lDataR = mNodeMarker_InR->GetAnimationToRecord();
	if (lDataR)
	{
		if( mPlayerControl.GetTransportMode() == kFBTransportPlay )
		{
			mHardware.GetRotation( lRot );
			switch( SamplingMode )
			{
				case kFBHardwareTimestamp:
				case kFBSoftwareTimestamp:
				{
					lDataR->KeyAdd(lTime, lRot);
				}
				break;

				case kFBHardwareFrequency:
				case kFBAutoFrequency:
				{
					lDataR->KeyAdd(lRot);
				}
				break;
			}
		}
	}
}

void ORDevice_Template::SetCandidates()
{
	double	lPos[3];
	double	lRot[3];

	mHardware.GetPosition( lPos );
	mHardware.GetRotation( lRot );

	mNodeMarker_InT->SetCandidate( lPos );
	mNodeMarker_InR->SetCandidate( lRot );
}

//--- FBX load/save tags
#define FBX_VERSION_TAG		"Version"
#define FBX_VERSION_VAL		100
#define FBX_COMMPARAM_TAG	"Communication"

/************************************************
 *	Store data in FBX.
 ************************************************/
bool ORDevice_Template::FbxStore(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
    //TBD store the data to FBX file, uncomment the codes 
/*
    if (pStoreWhat & kAttributes)
	{
		// Write version
		pFbxObject->FieldWriteI		(FBX_VERSION_TAG,FBX_VERSION_VAL);

		// Write communications settings
		pFbxObject->FieldWriteBegin	(FBX_COMMPARAM_TAG);
		{
		}
	    pFbxObject->FieldWriteEnd();
	}
*/
    return true;

}


/************************************************
 *	Retrieve data from FBX.
 ************************************************/
bool ORDevice_Template::FbxRetrieve(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
    //TBD store the data to FBX file, uncomment the codes 
/*
    int			Version;
	FBString	name;

	if (pStoreWhat & kAttributes)
	{
		// Get version
		Version	= pFbxObject->FieldReadI(FBX_VERSION_TAG);

		// Get communications settings
		if (pFbxObject->FieldReadBegin(FBX_COMMPARAM_TAG))
		{

			pFbxObject->FieldReadEnd();
		}
	}
*/
	return true;
}
