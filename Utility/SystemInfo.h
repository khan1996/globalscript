#pragma once
#include <ddraw.h>
#include <d3d8.h>
#include <math.h>

namespace SystemInfo
{
	//-----------------------------------------------------------------------------
	// Name: struct D3DModeInfo
	// Desc: Structure for holding information about a display mode
	//-----------------------------------------------------------------------------
	struct D3DModeInfo
	{
		DWORD      Width;      // Screen width in this mode
		DWORD      Height;     // Screen height in this mode
		D3DFORMAT  Format;     // Pixel format in this mode
		DWORD      dwBehavior; // Hardware / Software / Mixed vertex processing
		DWORD      dwRefresh[50];
		D3DFORMAT  DepthStencilFormat; // Which depth/stencil format to use with this mode
	};




	//-----------------------------------------------------------------------------
	// Name: struct D3DDeviceInfo
	// Desc: Structure for holding information about a Direct3D device, including
	//       a list of modes compatible with this device
	//-----------------------------------------------------------------------------
	struct D3DDeviceInfo
	{
		// Device data
		D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
		D3DCAPS8     d3dCaps;         // Capabilities of this device
		const TCHAR* strDesc;         // Name of this device
		BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

		// Modes for this device
		DWORD        dwNumModes;
		D3DModeInfo  modes[150];

		// Current state
		DWORD        dwCurrentMode;
		BOOL         bWindowed;
		D3DMULTISAMPLE_TYPE MultiSampleType;
	};

	struct CusD3DModeInfo
	{
		DWORD      Width;      // Screen width in this mode
		DWORD      Height;     // Screen height in this mode
		DWORD      m_dwNumFormat;
		D3DModeInfo  Format[10];
	};




	struct CusD3DDeviceInfo
	{
		// Device data
		D3DDEVTYPE   DeviceType;      // Reference, HAL, etc.
		D3DCAPS8     d3dCaps;         // Capabilities of this device
		const TCHAR* strDesc;         // Name of this device
		BOOL         bCanDoWindowed;  // Whether this device can work in windowed mode

		// Modes for this device
		DWORD        dwNumModes;
		CusD3DModeInfo  modes[150];

		// Current state
		DWORD        dwCurrentMode;
		BOOL         bWindowed;
		D3DMULTISAMPLE_TYPE MultiSampleType;
	};




	//-----------------------------------------------------------------------------
	// Name: struct D3DAdapterInfo
	// Desc: Structure for holding information about an adapter, including a list
	//       of devices available on this adapter
	//-----------------------------------------------------------------------------
	struct D3DAdapterInfo
	{
		// Adapter data
		D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
		D3DDISPLAYMODE d3ddmDesktop;      // Desktop display mode for this adapter

		// Devices for this adapter
		DWORD          dwNumDevices;
		D3DDeviceInfo  devices[5];

		// Current state
		DWORD          dwCurrentDevice;
	};

	/* Display Modes */
	typedef struct _CusD3DDISPLAYMODE
	{
		UINT            Width;
		UINT            Height;
		UINT            RefreshRate[50];
		D3DFORMAT       Format;
	} CusD3DDISPLAYMODE;

	typedef struct _sDiskInfo
	{
		char Letter[8];
		ULARGE_INTEGER m_lFreeBytesAvailableToCaller;    
		ULARGE_INTEGER m_lTotalBytes;
		ULARGE_INTEGER m_lTotalFreeBytes;
	} sDiskInfo;

	class CSystemInfo
	{
	protected:
		unsigned long m_dwMinDepthBits;			// 최소 뎁스버퍼 비트수
		unsigned long m_dwMinStencilBits;		// 최소 스텐실 버퍼 비트 수

		HRESULT ConfirmDevice(D3DCAPS8*,DWORD,D3DFORMAT);
		BOOL FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT *pDepthStencilFormat);

		OSVERSIONINFOEX m_osvi;
		BOOL m_bOsVersionInfoEx;
		MEMORYSTATUS m_stat;
		char m_strCPUInfo[8][MAX_PATH];
		unsigned long m_dwCPUSpeed;
		sDiskInfo m_sDiskInfo[20];

		char m_strWindowName[MAX_PATH];
		char m_strServiceVersion[MAX_PATH];

		char m_strSoundCard[MAX_PATH];
		char m_strSoundCardVersion[MAX_PATH];

		LPDIRECT3D8 m_lpD3D;

		DDCAPS m_ddHALCaps, m_ddHELCaps;

	public:
		unsigned long m_dwNumAdapters;
		D3DAdapterInfo m_Adapters[10];

		CusD3DDeviceInfo  m_d3dAvailableDevices[5];

		unsigned long m_dwCurrentVideoCard;
		unsigned long m_dwCurrentDevice;
		unsigned long m_dwCurrentResolution;
		unsigned long m_dwCurrentColor;
		unsigned long m_dwCurrentRefresh;

		CSystemInfo(void);
		~CSystemInfo(void);

		void SetDepthBits(unsigned long dwMinDepthBits) { m_dwMinDepthBits = dwMinDepthBits; }
		void SetStencilBits(unsigned long dwMinStencilBits) { m_dwMinStencilBits = dwMinStencilBits; }

		BOOL GetWindowInfo(void);
		BOOL GetMemoryInfo(void);
		BOOL GetCPUInfo(void);
		BOOL GetDiskInfo(void);
		BOOL GetSoundCardInfo(void);
		BOOL GetVideoCardInfo(void);

		void ReadCPUCounter(UINT64 *pCounter);

		HRESULT EnumulationD3D(void);
		void SetSetting(unsigned long dwWidth, unsigned long dwHeight, D3DFORMAT d3dFormat, unsigned long dwRefresh);

		BOOL GetAvailableDevice(unsigned long dwMinWidth, unsigned long dwMinHeight);
		D3DFORMAT GetFormat(unsigned long dwDepth);

		char *GetWindowName(void) {	return m_strWindowName; }
		double GetTotalPhysMem(void) { return (double)(m_stat.dwTotalPhys / 1024 / 1024.0); }
		unsigned long GetAvailPhysMem(void) { return (unsigned long)(m_stat.dwAvailPhys / 1024 / 1024); }
		unsigned long GetAvailPages(void) { return (unsigned long)(m_stat.dwAvailPageFile / 1024 / 1024); }
		char *GetCPUInfo(unsigned long dwNum) { return m_strCPUInfo[dwNum]; }
		double GetHDDFreeSpace(void) { return ((double)(m_sDiskInfo->m_lTotalFreeBytes.QuadPart) / 1024 / 1024 / 1024); }
		char *GetSoundCardName(void) { return m_strSoundCard; }
		unsigned long GetNumAdapters(void) { return m_dwNumAdapters; }
		char *GetGraphicCardName(unsigned long dwNum) { return m_Adapters[dwNum].d3dAdapterIdentifier.Description; }
		void GetResolutionByString(char *strFormat, char *strString);
		void GetColorByString(char *strFormat, char *strString);
		void GetRefreshByString(char *strFormat, char *strString);
		unsigned long GetCurrentWidth(void) { return m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Width; }
		unsigned long GetCurrentHeight(void) { return m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Height; }
		unsigned long GetCurrentColor(void) { return m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[m_dwCurrentColor].Format; }
		unsigned long GetCurrentRefresh(void) { return m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[m_dwCurrentColor].dwRefresh[m_dwCurrentRefresh]; }

		BOOL IsAblePages(unsigned long dwNeedPage) { return (((m_stat.dwAvailPageFile / 1024 / 1024) > dwNeedPage)? TRUE : FALSE); }
		BOOL IsTotalPhysMem(unsigned long dwNeedRam) { return ((ceil((double)(m_stat.dwTotalPhys / 1024 / 1024.0)) >= dwNeedRam)? TRUE : FALSE); }

		unsigned long GetAvailableShader(unsigned long dwPSVersion, unsigned long dwVSVersion);
		BOOL GetAvailableHiResTexture(unsigned long dwMinWidth, unsigned long dwMinHeight);
		BOOL GetAvailableCompressTexture(D3DFORMAT d3dFormat);
		unsigned long GetAbleWindowVersion(void);

		unsigned long GetPerformance(void);

		BOOL IsMinimum(void);
	};

};


