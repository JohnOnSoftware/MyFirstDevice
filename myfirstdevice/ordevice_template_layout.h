#ifndef __ORDEVICE_TEMPLATE_LAYOUT_H__
#define __ORDEVICE_TEMPLATE_LAYOUT_H__

#include "ordevice_template_device.h"

//TBD uncomment implementation and registration
class ORDevice_Template_Layout : public FBDeviceLayout
{
	FBDeviceLayoutDeclare( ORDevice_Template_Layout, FBDeviceLayout );
public:
	//--- FiLMBOX Creation/Destruction.
	virtual bool FBCreate();		//!< FiLMBOX Constructor.
	virtual void FBDestroy();		//!< FiLMBOX Destructor.

	// UI Management
	void	UICreate				();
	void		UICreateLayout0		();
	void		UICreateLayout1		();
	void	UIConfigure				();
	void		UIConfigureLayout0	();
	void		UIConfigureLayout1	();
	void	UIRefresh				();		// Idle refresh
	void	UIReset					();		// Reset from device values

	// Main Layout: Events
	void	EventDeviceStatusChange					( HISender pSender, HKEvent pEvent );
	void	EventUIIdle								( HISender pSender, HKEvent pEvent );
	void	EventTabPanelChange						( HISender pSender, HKEvent pEvent );

	// Layout 0: Events
	void	EventEditNumberSamplingRateChange		( HISender pSender, HKEvent pEvent );
	void	EventListSamplingTypeChange				( HISender pSender, HKEvent pEvent );
	void	EventButtonSetCandidateClick			( HISender pSender, HKEvent pEvent );
	void	EventButtonTestClick					( HISender pSender, HKEvent pEvent );

	// Layout 1: Events
	void	EventButtonSerialEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventListSerialPortChange				( HISender pSender, HKEvent pEvent );
	void	EventListSerialBaudRateChange			( HISender pSender, HKEvent pEvent );
	void	EventButtonNetworkEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventEditNetworkAddressChange			( HISender pSender, HKEvent pEvent );
	void	EventEditNetworkPortChange				( HISender pSender, HKEvent pEvent );
	void	EventListNetworkProtocolChange			( HISender pSender, HKEvent pEvent );
	void	EventButtonSimulatorEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventEditNumberSimulatorFrequencyChange	( HISender pSender, HKEvent pEvent );
	void	EventEditNumberSimulatorAmplitudeChange	( HISender pSender, HKEvent pEvent );

	// Layout configuration functions
	void	SelectCommunicationType( int pType );

private:
	FBTabPanel			mTabPanel;

	FBLayout			mLayoutGeneral;
		FBLabel				mLabelSamplingRate;
		FBEditNumber		mEditNumberSamplingRate;
		FBLabel				mLabelSamplingType;
		FBList				mListSamplingType;
		FBButton			mButtonSetCandidate;
		FBButton			mButtonTest;

	FBLayout			mLayoutCommunication;
		FBButton			mButtonSerialEnable;
			FBLabel				mLabelSerialPort;
			FBList				mListSerialPort;
			FBLabel				mLabelSerialBaudRate;
			FBList				mListSerialBaudRate;
		FBButton			mButtonNetworkEnable;
			FBLabel				mLabelNetworkAddress;
			FBEdit				mEditNetworkAddress;
			FBLabel				mLabelNetworkPort;
			FBEdit				mEditNetworkPort;
			FBLabel				mLabelNetworkProtocol;
			FBList				mListNetworkProtocol;
		FBButton			mButtonSimulatorEnable;
			FBLabel				mLabelSimulatorFrequency;
			FBEditNumber		mEditNumberSimulatorFrequency;
			FBLabel				mLabelSimulatorAmplitude;
			FBEditNumber		mEditNumberSimulatorAmplitude;

private:
	FBSystem			mSystem;		//!< System interface.
	ORDevice_Template*	mDevice;		//!< Handle onto device.
};

#endif /* __ORDEVICE_TEMPLATE_HARDWARE_H__ */
