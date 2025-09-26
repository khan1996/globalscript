
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <stdio.h>
#include "systeminfo.h"
#include "timer.h"
#include "GMMemory.h"

using namespace SystemInfo;

CSystemInfo::CSystemInfo(void)
{
	ZeroMemory(&m_osvi, sizeof(OSVERSIONINFOEX));
	m_bOsVersionInfoEx = FALSE;

	m_dwMinDepthBits = 0;
	m_dwMinStencilBits = 0;

	m_dwCurrentVideoCard = 0;
	m_dwCurrentDevice = 0;
	m_dwCurrentResolution = 0;
	m_dwCurrentColor = 0;
	m_dwCurrentRefresh = 0;
}

CSystemInfo::~CSystemInfo(void)
{
}

BOOL CSystemInfo::GetWindowInfo(void)
{
   // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
   // If that fails, try using the OSVERSIONINFO structure.

   m_osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   if( !(m_bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &m_osvi)) )
   {
      m_osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &m_osvi) ) 
         return FALSE;
   }

   switch (m_osvi.dwPlatformId)
   {
      // Test for the Windows NT product family.
      case VER_PLATFORM_WIN32_NT:

         // Test for the specific product family.
         if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
			 strcpy(m_strWindowName, "Microsoft Windows Server&nbsp;2003 family, ");

         if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1 )
            strcpy(m_strWindowName, "Microsoft Windows XP ");

         if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
            strcpy(m_strWindowName, "Microsoft Windows 2000 ");

         if ( m_osvi.dwMajorVersion <= 4 )
            strcpy(m_strWindowName, "Microsoft Windows NT ");

         // Test for specific product on Windows NT 4.0 SP6 and later.
         if( m_bOsVersionInfoEx )
         {
            // Test for the workstation type.
            if ( m_osvi.wProductType == VER_NT_WORKSTATION )
            {
               if( m_osvi.dwMajorVersion == 4 )
                  strcat(m_strWindowName, "Workstation 4.0 " );
               else if( m_osvi.wSuiteMask & VER_SUITE_PERSONAL )
                  strcat(m_strWindowName, "Home Edition " );
               else
                  strcat(m_strWindowName, "Professional " );
            }
            
            // Test for the server type.
            else if ( m_osvi.wProductType == VER_NT_SERVER )
            {
               if( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
               {
                  if( m_osvi.wSuiteMask & VER_SUITE_DATACENTER )
                     strcat(m_strWindowName, "Datacenter Edition " );
                  else if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                     strcat(m_strWindowName, "Enterprise Edition " );
                  else if ( m_osvi.wSuiteMask == VER_SUITE_BLADE )
                     strcat(m_strWindowName, "Web Edition " );
                  else
                     strcat(m_strWindowName, "Standard Edition " );
               } else if( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
               {
                  if( m_osvi.wSuiteMask & VER_SUITE_DATACENTER )
                     strcat(m_strWindowName, "Datacenter Server " );
                  else if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                     strcat(m_strWindowName, "Advanced Server " );
                  else
                     strcat(m_strWindowName, "Server " );
               } else  // Windows NT 4.0 
               {
                  if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                     strcat(m_strWindowName, "Server 4.0, Enterprise Edition " );
                  else
                     strcat(m_strWindowName, "Server 4.0 " );
               }
            }
         } else  // Test for specific product on Windows NT 4.0 SP5 and earlier
         {
            HKEY hKey;
            char szProductType[80];
            DWORD dwBufLen=80;
            LONG lRet;

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey );
            if( lRet != ERROR_SUCCESS )
               return FALSE;

            lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
               (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > 80) )
               return FALSE;

            RegCloseKey( hKey );

            if ( lstrcmpi( "WINNT", szProductType) == 0 )
               strcat(m_strWindowName, "Workstation " );
            if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
               strcat(m_strWindowName, "Server " );
            if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
               strcat(m_strWindowName, "Advanced Server " );

			char strTemp[MAX_PATH];
            sprintf( strTemp, "%d.%d ", m_osvi.dwMajorVersion, m_osvi.dwMinorVersion );
            strcat(m_strWindowName, strTemp);
        }

      // Display service pack (if any) and build number.

         if( m_osvi.dwMajorVersion == 4 && lstrcmpi( m_osvi.szCSDVersion, "Service Pack 6" ) == 0 )
         {
            HKEY hKey;
            LONG lRet;

            // Test for SP6 versus SP6a.
            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
               0, KEY_QUERY_VALUE, &hKey );
            if( lRet == ERROR_SUCCESS )
               sprintf(m_strServiceVersion, "Service Pack 6a (Build %d)\n", m_osvi.dwBuildNumber & 0xFFFF );         
            else // Windows NT 4.0 prior to SP6a
            {
               sprintf(m_strServiceVersion, "%s (Build %d)", m_osvi.szCSDVersion, m_osvi.dwBuildNumber & 0xFFFF);
            }

            RegCloseKey( hKey );
         }
         else // Windows NT 3.51 and earlier or Windows 2000 and later
         {
            sprintf(m_strServiceVersion, "%s (Build %d)", m_osvi.szCSDVersion, m_osvi.dwBuildNumber & 0xFFFF);
         }


         break;

      // Test for the Windows 95 product family.
      case VER_PLATFORM_WIN32_WINDOWS:

         if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 0)
         {
             strcpy(m_strWindowName, "Microsoft Windows 95 ");
             if ( m_osvi.szCSDVersion[1] == 'C' || m_osvi.szCSDVersion[1] == 'B' )
                strcpy(m_strServiceVersion, "OSR2 " );
         } 

         if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 10)
         {
             strcpy(m_strWindowName, "Microsoft Windows 98 ");
             if ( m_osvi.szCSDVersion[1] == 'A' )
                strcpy(m_strServiceVersion, "SE " );
         } 

         if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 90)
         {
             strcpy(m_strWindowName, "Microsoft Windows Millennium Edition");
         } 
         break;

      case VER_PLATFORM_WIN32s:

         strcpy(m_strWindowName, "Microsoft Win32s");
         break;
   }

   return TRUE;
}

typedef HRESULT (WINAPI* DirectDrawCreate_PROC) (GUID FAR * lpGuid, LPVOID  *lplpDD, IUnknown FAR *pUnkOuter);

BOOL CSystemInfo::GetVideoCardInfo(void)
{
	HMODULE hLibrary = LoadLibrary("ddraw.dll");
	if(!hLibrary)
	{
		MessageBox(NULL, "DirectX not Installed.", "System Detect", MB_OK);
	}

	LPDIRECTDRAW lpDDraw;
	DirectDrawCreate_PROC ddCreate;
	ddCreate = (DirectDrawCreate_PROC)GetProcAddress(hLibrary, "DirectDrawCreate");

	if(!ddCreate)
	{
		MessageBox(NULL, "DirectX not Installed.", "System Detect", MB_OK);
	}

	HRESULT hr = (ddCreate)(NULL, (VOID**)&lpDDraw, NULL);

	if(FAILED(hr))
	{
		MessageBox(NULL, "DirectX not Installed.", "System Detect", MB_OK);
	}

	ZeroMemory(&m_ddHALCaps, sizeof(m_ddHALCaps));
	m_ddHALCaps.dwSize = sizeof(m_ddHALCaps);
	ZeroMemory(&m_ddHELCaps, sizeof(m_ddHELCaps));
	m_ddHELCaps.dwSize = sizeof(m_ddHELCaps);
	hr = lpDDraw->GetCaps(&m_ddHALCaps, &m_ddHELCaps);

	lpDDraw->Release();
	FreeLibrary(hLibrary);

	return TRUE;
}

BOOL CSystemInfo::GetMemoryInfo(void)
{
	GlobalMemoryStatus(&m_stat);

	return TRUE;
}

void CSystemInfo::ReadCPUCounter(UINT64 *pCounter)
{
	_asm
	{
		RDTSC
		mov edi, pCounter
		mov DWORD PTR [edi], eax
		mov DWORD PTR [edi+4], edx
	};
}

BOOL CSystemInfo::GetCPUInfo(void)
{
	HKEY hKey, hSubKey1;
	unsigned long dwSubKeyNum = 0;
	
	char szParentKeyName[MAX_PATH] = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor";
	char szSubKeyName1[MAX_PATH];
	char szTempName[MAX_PATH];
	char szValue[MAX_PATH];

	unsigned long dwSize = MAX_PATH;

	char *temp;
	temp = new char[MAX_PATH];

	int nCnt = 0;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, szParentKeyName, 0, KEY_READ, &hKey))
	{
		if (ERROR_SUCCESS == RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwSubKeyNum, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
		{
			for (unsigned long nIndex = 0; nIndex < dwSubKeyNum; ++nIndex)
			{
				if( ERROR_SUCCESS == RegEnumKey(hKey, nIndex, szTempName, MAX_PATH) )
				{

					strcpy(szSubKeyName1, szParentKeyName);
					strcat(szSubKeyName1, "\\");
					strcat(szSubKeyName1, szTempName);
					if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szSubKeyName1, 0, KEY_READ, &hSubKey1))
						continue;

					if (ERROR_SUCCESS == RegQueryValueEx(hSubKey1, "ProcessorNameString", 0, NULL, (LPBYTE)szValue, &dwSize))	
					{
						strcpy(temp, szValue);
						temp = strrev(temp);
						int len = (int)strlen(temp) - 1;
						while(len)
						{
							if(temp[len] == ' ') temp[len] = 0; else break;
							--len;
						}
						temp = strrev(temp);
						strcpy(szValue, temp);

						strcpy( m_strCPUInfo[nCnt], szValue );
						nCnt++;
						RegCloseKey( hSubKey1 );
						continue;
					}

					RegCloseKey( hSubKey1 );
				}
			}
		}
		RegCloseKey( hKey );
	}

	delete[] temp;

	UINT64 startTime, endTime;
	gaia::cTimer localTimer;

	localTimer.start();
	ReadCPUCounter(&startTime);
	Sleep(100);
	ReadCPUCounter(&endTime);
	localTimer.stop();

	UINT64 sampleData = endTime - startTime;
	UINT32 elapseMilliseconds = localTimer.elapsedMilliseconds();
	m_dwCPUSpeed = (unsigned long)sampleData / elapseMilliseconds;

	return TRUE;
}

BOOL CSystemInfo::GetDiskInfo(void)
{
	int nDiskCheck;
	unsigned int nDiskType, nCount = 0;
	char Drive[10];

	for(nDiskCheck = 'C'; nDiskCheck <= 'Z'; nDiskCheck++)
	{
		wsprintf(Drive, "%c:\\", nDiskCheck);
		nDiskType = GetDriveType(Drive);

		if(nDiskType == DRIVE_FIXED)   // HDD 
		{
			strcpy(m_sDiskInfo[nCount].Letter, Drive);
			int res = GetDiskFreeSpaceEx( m_sDiskInfo[nCount].Letter, &m_sDiskInfo[nCount].m_lFreeBytesAvailableToCaller,
				&m_sDiskInfo[nCount].m_lTotalBytes, &m_sDiskInfo[nCount].m_lTotalFreeBytes );
			nCount++;
		}
	}

	return TRUE;
}

BOOL CSystemInfo::GetSoundCardInfo(void)
{
	MMRESULT mmRes = MMSYSERR_NOERROR;
	UINT cbCaps;
	WAVEOUTCAPS wavCaps;

	// Get the number of audio output devices installed on your system.
//	numDevices = waveOutGetNumDevs ();

	// If the device is present, only then we will proceed to get other information.
	if(waveOutGetNumDevs() > 0)
	{
		cbCaps = sizeof (AUXCAPS);
		mmRes = waveOutGetDevCaps(0, &wavCaps, cbCaps);
		if(mmRes == MMSYSERR_NOERROR) 
		{
			strcpy(m_strSoundCard, wavCaps.szPname);

			return TRUE;
		}
	}

	return FALSE;
}

typedef IDirect3D8 * (WINAPI* Direct3DCreate8_PROC) (UINT SDKVersion);

HRESULT CSystemInfo::EnumulationD3D(void)
{
	HMODULE hLibrary = LoadLibrary("d3d8.dll");
	if(!hLibrary)
	{
		MessageBox(NULL, "DirectX 8.1 not Installed.", "System Detect", MB_OK);
	}

	Direct3DCreate8_PROC d3dCreate;
	d3dCreate = (Direct3DCreate8_PROC)GetProcAddress(hLibrary, "Direct3DCreate8");

	if(!d3dCreate)
	{
		MessageBox(NULL, "DirectX 8.1 not Installed.", "System Detect", MB_OK);
	}

	m_lpD3D = (d3dCreate)(D3D_SDK_VERSION);

	const DWORD dwNumDeviceTypes = 2;
    const TCHAR* strDeviceDescs[] = { "HAL", "REF" };
    const D3DDEVTYPE DeviceTypes[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_REF };

    BOOL bHALExists = FALSE;
    BOOL bHALIsWindowedCompatible = FALSE;
    BOOL bHALIsDesktopCompatible = FALSE;
    BOOL bHALIsSampleCompatible = FALSE;

    // Loop through all the adapters on the system (usually, there's just one
    // unless more than one graphics card is present).
	m_dwNumAdapters = 0;
    for( UINT iAdapter = 0; iAdapter < m_lpD3D->GetAdapterCount(); iAdapter++ )
    {
        // Fill in adapter info
        D3DAdapterInfo* pAdapter  = &m_Adapters[m_dwNumAdapters];
        m_lpD3D->GetAdapterIdentifier( iAdapter, 0, &pAdapter->d3dAdapterIdentifier );
        m_lpD3D->GetAdapterDisplayMode( iAdapter, &pAdapter->d3ddmDesktop );
        pAdapter->dwNumDevices    = 0;
        pAdapter->dwCurrentDevice = 0;

        // Enumerate all display modes on this adapter
        CusD3DDISPLAYMODE modes[100];
        D3DFORMAT      formats[20];
        DWORD dwNumFormats      = 0;
        DWORD dwNumModes        = 0;
        DWORD dwNumAdapterModes = m_lpD3D->GetAdapterModeCount( iAdapter );

        // Add the adapter's current desktop format to the list of formats
        formats[dwNumFormats++] = pAdapter->d3ddmDesktop.Format;

		DWORD dwRefreshCount = 0;
		ZeroMemory(&modes[0].RefreshRate, sizeof(modes[0].RefreshRate));
        for( UINT iMode = 0; iMode < dwNumAdapterModes; iMode++ )
        {
            // Get the display mode attributes
            D3DDISPLAYMODE DisplayMode;
            m_lpD3D->EnumAdapterModes( iAdapter, iMode, &DisplayMode );

            // Filter out low-resolution modes
            if( DisplayMode.Width  < 640 || DisplayMode.Height < 400 )
                continue;

            // Check if the mode already exists (to filter out refresh rates)
			DWORD m=0L;
            for(; m<dwNumModes; m++ )
            {
                if( ( modes[m].Width  == DisplayMode.Width  ) &&
                    ( modes[m].Height == DisplayMode.Height ) &&
                    ( modes[m].Format == DisplayMode.Format ) )
                    break;
            }

            // If we found a new mode, add it to the list of modes
            if( m == dwNumModes )
            {
				dwRefreshCount = 0;
				ZeroMemory(&modes[dwNumModes].RefreshRate, sizeof(modes[dwNumModes].RefreshRate));
				modes[dwNumModes].RefreshRate[dwRefreshCount++] = DisplayMode.RefreshRate;
                modes[dwNumModes].Width       = DisplayMode.Width;
                modes[dwNumModes].Height      = DisplayMode.Height;
                modes[dwNumModes].Format      = DisplayMode.Format;
                dwNumModes++;

                // Check if the mode's format already exists
				DWORD f=0;
                for(; f<dwNumFormats; f++ )
                {
                    if( DisplayMode.Format == formats[f] )
                        break;
                }

                // If the format is new, add it to the list
                if( f== dwNumFormats )
                    formats[dwNumFormats++] = DisplayMode.Format;
            } else
			{
				modes[m].RefreshRate[dwRefreshCount++] = DisplayMode.RefreshRate;
			}
        }

        // Sort the list of display modes (by format, then width, then height)
 //       qsort( modes, dwNumModes, sizeof(CusD3DDISPLAYMODE), SortModesCallback );

        // Add devices to adapter
        for( UINT iDevice = 0; iDevice < dwNumDeviceTypes; iDevice++ )
        {
            // Fill in device info
            D3DDeviceInfo* pDevice;
            pDevice                 = &pAdapter->devices[pAdapter->dwNumDevices];
            pDevice->DeviceType     = DeviceTypes[iDevice];
            m_lpD3D->GetDeviceCaps( iAdapter, DeviceTypes[iDevice], &pDevice->d3dCaps );
            pDevice->strDesc        = strDeviceDescs[iDevice];
            pDevice->dwNumModes     = 0;
            pDevice->dwCurrentMode  = 0;
            pDevice->bCanDoWindowed = FALSE;
            pDevice->bWindowed      = FALSE;
            pDevice->MultiSampleType = D3DMULTISAMPLE_NONE;

            // Examine each format supported by the adapter to see if it will
            // work with this device and meets the needs of the application.
            BOOL  bFormatConfirmed[20];
            DWORD dwBehavior[20];
            D3DFORMAT fmtDepthStencil[20];

            for( DWORD f=0; f<dwNumFormats; f++ )
            {
                bFormatConfirmed[f] = FALSE;
                fmtDepthStencil[f] = D3DFMT_UNKNOWN;

                // Skip formats that cannot be used as render targets on this device
                if( FAILED( m_lpD3D->CheckDeviceType( iAdapter, pDevice->DeviceType,
                                                     formats[f], formats[f], FALSE ) ) )
                    continue;

                if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                {
                    // This system has a HAL device
                    bHALExists = TRUE;

                    if( pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED )
                    {
                        // HAL can run in a window for some mode
                        bHALIsWindowedCompatible = TRUE;

                        if( f == 0 )
                        {
                            // HAL can run in a window for the current desktop mode
                            bHALIsDesktopCompatible = TRUE;
                        }
                    }
                }

                // Confirm the device/format for HW vertex processing
                if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT )
                {
                    if( pDevice->d3dCaps.DevCaps&D3DDEVCAPS_PUREDEVICE )
                    {
						/*
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING |
                                        D3DCREATE_PUREDEVICE;
										*/
						dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = FALSE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_HARDWARE_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }

                    if ( FALSE == bFormatConfirmed[f] )
                    {
                        dwBehavior[f] = D3DCREATE_MIXED_VERTEXPROCESSING;

                        if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                      formats[f] ) ) )
                            bFormatConfirmed[f] = TRUE;
                    }
                }

                // Confirm the device/format for SW vertex processing
                if( FALSE == bFormatConfirmed[f] )
                {
                    dwBehavior[f] = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

                    if( SUCCEEDED( ConfirmDevice( &pDevice->d3dCaps, dwBehavior[f],
                                                  formats[f] ) ) )
                        bFormatConfirmed[f] = TRUE;
                }

                // Find a suitable depth/stencil buffer format for this device/format
                if( bFormatConfirmed[f])
                {
                    if( !FindDepthStencilFormat( iAdapter, pDevice->DeviceType,
                        formats[f], &fmtDepthStencil[f] ) )
                    {
                        bFormatConfirmed[f] = FALSE;
                    }
                }
            }

            // Add all enumerated display modes with confirmed formats to the
            // device's list of valid modes
            for( DWORD m=0L; m<dwNumModes; m++ )
            {
                for( DWORD f=0; f<dwNumFormats; f++ )
                {
                    if( modes[m].Format == formats[f] )
                    {
                        if( bFormatConfirmed[f] == TRUE )
                        {
                            // Add this mode to the device's list of valid modes
                            pDevice->modes[pDevice->dwNumModes].Width      = modes[m].Width;
                            pDevice->modes[pDevice->dwNumModes].Height     = modes[m].Height;
                            pDevice->modes[pDevice->dwNumModes].Format     = modes[m].Format;
                            pDevice->modes[pDevice->dwNumModes].dwBehavior = dwBehavior[f];
                            pDevice->modes[pDevice->dwNumModes].DepthStencilFormat = fmtDepthStencil[f];

							for( DWORD r = 0; r < 50; ++r )
							{
								pDevice->modes[pDevice->dwNumModes].dwRefresh[r] = modes[m].RefreshRate[r];
							}

                            pDevice->dwNumModes++;

                            if( pDevice->DeviceType == D3DDEVTYPE_HAL )
                                bHALIsSampleCompatible = TRUE;
                        }
                    }
                }
            }

            // Select any 640x480 mode for default (but prefer a 16-bit mode)
            for(DWORD m=0; m<pDevice->dwNumModes; m++ )
            {
                if( pDevice->modes[m].Width==640 && pDevice->modes[m].Height==480 )
                {
                    pDevice->dwCurrentMode = m;
                    if( pDevice->modes[m].Format == D3DFMT_R5G6B5 ||
                        pDevice->modes[m].Format == D3DFMT_X1R5G5B5 ||
                        pDevice->modes[m].Format == D3DFMT_A1R5G5B5 )
                    {
                        break;
                    }
                }
            }

            // Check if the device is compatible with the desktop display mode
            // (which was added initially as formats[0])
            if( bFormatConfirmed[0] && (pDevice->d3dCaps.Caps2 & D3DCAPS2_CANRENDERWINDOWED) )
            {
                pDevice->bCanDoWindowed = TRUE;
                pDevice->bWindowed      = TRUE;
            }

            // If valid modes were found, keep this device
            if( pDevice->dwNumModes > 0 )
                pAdapter->dwNumDevices++;
        }

        // If valid devices were found, keep this adapter
        if( pAdapter->dwNumDevices > 0 )
            m_dwNumAdapters++;
    }

    // Return an error if no compatible devices were found
    if( 0L == m_dwNumAdapters )
	{
		m_lpD3D->Release();
		FreeLibrary(hLibrary);
		return S_OK;
	}

    // Pick a default device that can render into a window
    // (This code assumes that the HAL device comes before the REF
    // device in the device array).
	m_lpD3D->Release();
//	EnumSound();

    for( DWORD a=0; a<m_dwNumAdapters; a++ )
    {
        for( DWORD d=0; d < m_Adapters[a].dwNumDevices; d++ )
        {
            if( m_Adapters[a].devices[d].bWindowed )
            {
                m_Adapters[a].dwCurrentDevice = d;

				FreeLibrary(hLibrary);
                return S_OK;
            }
        }
    }    	

	FreeLibrary(hLibrary);
	return S_OK;
}

void CSystemInfo::SetSetting(unsigned long dwWidth, unsigned long dwHeight, D3DFORMAT d3dFormat, unsigned long dwRefresh)
{
	for(unsigned long j = 0; j < m_Adapters[m_dwCurrentVideoCard].dwNumDevices; ++j)
	{
		if(!strcmp(m_d3dAvailableDevices[j].strDesc, "HAL"))
		{
			for(unsigned long k = 0; k < m_d3dAvailableDevices[j].dwNumModes; ++k)
			{
				if(dwWidth == m_d3dAvailableDevices[j].modes[k].Width &&
					dwHeight == m_d3dAvailableDevices[j].modes[k].Height)
				{
					for(unsigned long h = 0; h < m_d3dAvailableDevices[j].modes[k].m_dwNumFormat; ++h)
					{
						if(d3dFormat == m_d3dAvailableDevices[j].modes[k].Format[h].Format)
						{
							for(unsigned long l = 0; l < 50; ++l)
							{
								if(dwRefresh == m_d3dAvailableDevices[j].modes[k].Format[h].dwRefresh[l])
								{
									m_dwCurrentDevice = j;
									m_dwCurrentResolution = k;
									m_dwCurrentColor = h;
									m_dwCurrentRefresh = l;

									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSystemInfo::GetResolutionByString(char *strFormat, char *strString)
{
	sprintf(strString, strFormat, GetCurrentWidth(), GetCurrentHeight());
}

void CSystemInfo::GetColorByString(char *strFormat, char *strString)
{
	unsigned long dwColor;
	switch(m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[m_dwCurrentColor].Format)
	{
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4:
		case D3DFMT_A8L8:
			dwColor = 16;
			break;

		case D3DFMT_R8G8B8:
			dwColor = 24;
			break;

		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
		case D3DFMT_A8R3G3B2:
		case D3DFMT_X4R4G4B4:
		case D3DFMT_A2B10G10R10:
		case D3DFMT_G16R16:
			dwColor = 32;
			break;

		default:
			dwColor = 16;
			break;
	}

	sprintf(strString, strFormat, dwColor);
}

void CSystemInfo::GetRefreshByString(char *strFormat, char *strString)
{
	sprintf(strString, strFormat, m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[m_dwCurrentColor].dwRefresh[m_dwCurrentRefresh]);
}

D3DFORMAT CSystemInfo::GetFormat(unsigned long dwDepth)
{
	D3DFORMAT d3dResult;
	for(unsigned int i = 0; i < 10; ++i)
	{
		if(m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[i].Format != 0)
		{
			switch(m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[i].Format)
			{
				case D3DFMT_R5G6B5:
				case D3DFMT_X1R5G5B5:
				case D3DFMT_A1R5G5B5:
				case D3DFMT_A4R4G4B4:
				case D3DFMT_A8L8:
					if(dwDepth == 16)
					{
						d3dResult = m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[i].Format;

						return d3dResult;
					}
					break;

				case D3DFMT_R8G8B8:
					if(dwDepth == 24)
					{
						d3dResult = m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[i].Format;

						return d3dResult;
					}
					break;

				case D3DFMT_A8R8G8B8:
				case D3DFMT_X8R8G8B8:
				case D3DFMT_A8R3G3B2:
				case D3DFMT_X4R4G4B4:
				case D3DFMT_A2B10G10R10:
				case D3DFMT_G16R16:
					if(dwDepth == 32)
					{
						d3dResult = m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[i].Format;

						return d3dResult;
					}
					break;
			}
		}
	}

	return (D3DFORMAT)0;
}

BOOL CSystemInfo::FindDepthStencilFormat(UINT iAdapter, D3DDEVTYPE DeviceType, D3DFORMAT TargetFormat, D3DFORMAT *pDepthStencilFormat)
{
	if( m_dwMinDepthBits <= 16 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_lpD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
        {
            if( SUCCEEDED( m_lpD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D16 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D16;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 15 && m_dwMinStencilBits <= 1 )
    {
        if( SUCCEEDED( m_lpD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D15S1 ) ) )
        {
            if( SUCCEEDED( m_lpD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D15S1 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D15S1;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_lpD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8 ) ) )
        {
            if( SUCCEEDED( m_lpD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 8 )
    {
        if( SUCCEEDED( m_lpD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24S8 ) ) )
        {
            if( SUCCEEDED( m_lpD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24S8 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24S8;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 24 && m_dwMinStencilBits <= 4 )
    {
        if( SUCCEEDED( m_lpD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X4S4 ) ) )
        {
            if( SUCCEEDED( m_lpD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D24X4S4 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D24X4S4;
                return TRUE;
            }
        }
    }

    if( m_dwMinDepthBits <= 32 && m_dwMinStencilBits == 0 )
    {
        if( SUCCEEDED( m_lpD3D->CheckDeviceFormat( iAdapter, DeviceType,
            TargetFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32 ) ) )
        {
            if( SUCCEEDED( m_lpD3D->CheckDepthStencilMatch( iAdapter, DeviceType,
                TargetFormat, TargetFormat, D3DFMT_D32 ) ) )
            {
                *pDepthStencilFormat = D3DFMT_D32;
                return TRUE;
            }
        }
    }

    return FALSE;
}

HRESULT CSystemInfo::ConfirmDevice(D3DCAPS8 *, DWORD, D3DFORMAT)
{
	return S_OK;
}

BOOL CSystemInfo::GetAvailableDevice(unsigned long dwMinWidth, unsigned long dwMinHeight)
{
	unsigned long dwDevices, dwModes, dwFormat, dwCount;

	ZeroMemory(m_d3dAvailableDevices, sizeof(m_d3dAvailableDevices));
	for(dwDevices = 0; dwDevices < m_Adapters[m_dwCurrentVideoCard].dwNumDevices; ++dwDevices)
	{
		m_d3dAvailableDevices[dwDevices].bCanDoWindowed = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].bCanDoWindowed;
		m_d3dAvailableDevices[dwDevices].bWindowed = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].bWindowed;
		m_d3dAvailableDevices[dwDevices].d3dCaps = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].d3dCaps;
		m_d3dAvailableDevices[dwDevices].DeviceType = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].DeviceType;
		m_d3dAvailableDevices[dwDevices].dwCurrentMode = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].dwCurrentMode;
		m_d3dAvailableDevices[dwDevices].dwNumModes = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].dwNumModes;
		m_d3dAvailableDevices[dwDevices].MultiSampleType = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].MultiSampleType;
		m_d3dAvailableDevices[dwDevices].strDesc = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].strDesc;

		dwCount = 0;
		for(dwModes = 0; dwModes < m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].dwNumModes; ++dwModes)
		{
// 해상도 해제
//			if(m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width >= dwMinWidth &&
//				m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Height >= dwMinHeight)
			if(m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width >= dwMinWidth &&
				m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Height >= dwMinHeight &&
				(m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width == 800 ||
				m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width == 1024 ||
				m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width == 1280 ||
				m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width == 1600) &&
				(m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width * 3 == m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Height * 4))
			{
				for(dwFormat = 0; dwFormat < dwCount; ++dwFormat)
				{
					if(m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width == m_d3dAvailableDevices[dwDevices].modes[dwFormat].Width &&
						m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Height == m_d3dAvailableDevices[dwDevices].modes[dwFormat].Height)
					{
						break;
					}
				}

				if(dwFormat == dwCount)
				{
					m_d3dAvailableDevices[dwDevices].modes[dwCount].m_dwNumFormat = 1;
					m_d3dAvailableDevices[dwDevices].modes[dwCount].Width = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Width;
					m_d3dAvailableDevices[dwDevices].modes[dwCount].Height = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes].Height;
					m_d3dAvailableDevices[dwDevices].modes[dwCount].Format[0] = m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes];
					++dwCount;
				} else
				{
					m_d3dAvailableDevices[dwDevices].modes[dwFormat].Format[m_d3dAvailableDevices[dwDevices].modes[dwFormat].m_dwNumFormat]	= m_Adapters[m_dwCurrentVideoCard].devices[dwDevices].modes[dwModes];
					++m_d3dAvailableDevices[dwDevices].modes[dwFormat].m_dwNumFormat;
				}
			}
		}
		m_d3dAvailableDevices[dwDevices].dwNumModes = dwCount;
		m_d3dAvailableDevices[dwDevices].dwCurrentMode = 0;
	}

	return TRUE;
}

// 리턴값
//		0 - 가능
//		1 - 픽셀 쉐이더 버젼이 낮다
//		2 - 버텍스 쉐이더 버젼이 낮다.
//		3 - 픽쉘 세이더와 버텍스 쉐이더의 버젼이 낮다.
unsigned long CSystemInfo::GetAvailableShader(unsigned long dwPSVersion, unsigned long dwVSVersion)
{
	unsigned long dwResult = 0;

	if(m_Adapters[m_dwCurrentVideoCard].devices[m_dwCurrentDevice].d3dCaps.PixelShaderVersion < dwPSVersion)
	{
		dwResult += 1;
	}

	if(m_Adapters[m_dwCurrentVideoCard].devices[m_dwCurrentDevice].d3dCaps.VertexShaderVersion < dwVSVersion)
	{
		dwResult += 2;
	}

	return dwResult;
}

BOOL CSystemInfo::GetAvailableHiResTexture(unsigned long dwMinWidth, unsigned long dwMinHeight)
{
	if(m_Adapters[m_dwCurrentVideoCard].devices[m_dwCurrentDevice].d3dCaps.MaxTextureWidth > dwMinWidth &&
        m_Adapters[m_dwCurrentVideoCard].devices[m_dwCurrentDevice].d3dCaps.MaxTextureHeight > dwMinHeight)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CSystemInfo::GetAvailableCompressTexture(D3DFORMAT d3dFormat)
{
	HMODULE hLibrary = LoadLibrary("d3d8.dll");
	if(!hLibrary)
	{
		MessageBox(NULL, "DirectX 8.1 not Installed.", "System Detect", MB_OK);
	}

	Direct3DCreate8_PROC d3dCreate;
	d3dCreate = (Direct3DCreate8_PROC)GetProcAddress(hLibrary, "Direct3DCreate8");

	if(!d3dCreate)
	{
		MessageBox(NULL, "DirectX 8.1 not Installed.", "System Detect", MB_OK);
	}

	m_lpD3D = (d3dCreate)(D3D_SDK_VERSION);

	if(SUCCEEDED(m_lpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		m_d3dAvailableDevices[m_dwCurrentDevice].modes[m_dwCurrentResolution].Format[m_dwCurrentColor].Format, 0,
		D3DRTYPE_TEXTURE, d3dFormat)))
	{
		FreeLibrary(hLibrary);
		return TRUE;
	}

	m_lpD3D->Release();
	FreeLibrary(hLibrary);

	return FALSE;
}

BOOL CSystemInfo::IsMinimum(void)
{
	// 최소 메모리 부족 (256MB).
	if(m_stat.dwTotalPhys <= 268435456)
	{
		return FALSE;
	}
	// 최소 CPU Mhz 부족 (800 Mhz).
	if(m_dwCPUSpeed <= 800000)
	{
		return FALSE;
	}
	// 최소 비디오 메모리 부족 (32MB)
	if(m_ddHALCaps.dwVidMemTotal <= 33554432)
	{
		return FALSE;
	}
	return TRUE;
}


// 리턴값
//		0 - Failed
//		1 - 최저 사양
//		2 - 중간 사양
//		3 - 최고 사양
unsigned long CSystemInfo::GetPerformance(void)
{
	unsigned long dwPerformance = 0;
	unsigned long dwCPUPerformance = 0;
	unsigned long dwSysMemPerformance = 0;
	unsigned long dwVidMemPerformance = 0;

	/*다이렉트 X의 버젼과 최소 해상도, 최소 운영체제, 최소 메모리*/

	// CPU 속도
	if(m_dwCPUSpeed <= 900000)								// 900 MHz
	{
		dwCPUPerformance = 1;
	} else if(m_dwCPUSpeed <= 1500000)						// 1.5 GHz
	{
		dwCPUPerformance = 2;
	} else
	{
		dwCPUPerformance = 3;
	}

	// 그래픽 카드 램 용량
	if(m_ddHALCaps.dwVidMemTotal <= 100663296)				// 96 MB
	{
		dwVidMemPerformance = 1;
	} else if(m_ddHALCaps.dwVidMemTotal <= 201326592)		// 192 MB
	{
		dwVidMemPerformance = 2;
	} else
	{
		dwVidMemPerformance = 3;
	}

	// 램 용량
	if(m_stat.dwTotalPhys <= 402653184)						// 384 MB
	{
		dwSysMemPerformance = 1;
	} else if(m_stat.dwTotalPhys <= 805306368)				// 768 MB
	{
		dwSysMemPerformance = 2;
	} else
	{
		dwSysMemPerformance = 3;
	}

	// 그래픽 카드 종류

	dwPerformance = (unsigned long)(((dwCPUPerformance / 3.0 * 4 / 10) +
		(dwSysMemPerformance / 3.0 * 3 / 10) +
		(dwVidMemPerformance / 3.0 * 3 / 10)) * 3);

	return dwPerformance;
}

unsigned long CSystemInfo::GetAbleWindowVersion(void)
{
	unsigned long dwVersion = 0;

	switch (m_osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
		case VER_PLATFORM_WIN32_NT:

			// Test for the specific product family.
//			if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
//				strcpy(m_strWindowName, "Microsoft Windows Server&nbsp;2003 family, ");

			if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 1 )
				dwVersion = 4;
//			strcpy(m_strWindowName, "Microsoft Windows XP ");

			if ( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
				dwVersion = 3;
//			strcpy(m_strWindowName, "Microsoft Windows 2000 ");

//			if ( m_osvi.dwMajorVersion <= 4 )
//			strcpy(m_strWindowName, "Microsoft Windows NT ");

			// Test for specific product on Windows NT 4.0 SP6 and later.
			if( m_bOsVersionInfoEx )
			{
				// Test for the workstation type.
/*				if ( m_osvi.wProductType == VER_NT_WORKSTATION )
				{
					if( m_osvi.dwMajorVersion == 4 )
						strcat(m_strWindowName, "Workstation 4.0 " );
					else if( m_osvi.wSuiteMask & VER_SUITE_PERSONAL )
						strcat(m_strWindowName, "Home Edition " );
					else
						strcat(m_strWindowName, "Professional " );
				}
		        
				// Test for the server type.
				else*/ if ( m_osvi.wProductType == VER_NT_SERVER )
				{
					dwVersion = 0;
/*					if( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 2 )
					{
						if( m_osvi.wSuiteMask & VER_SUITE_DATACENTER )
							strcat(m_strWindowName, "Datacenter Edition " );
						else if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							strcat(m_strWindowName, "Enterprise Edition " );
						else if ( m_osvi.wSuiteMask == VER_SUITE_BLADE )
							strcat(m_strWindowName, "Web Edition " );
						else
							strcat(m_strWindowName, "Standard Edition " );
					} else if( m_osvi.dwMajorVersion == 5 && m_osvi.dwMinorVersion == 0 )
					{
						if( m_osvi.wSuiteMask & VER_SUITE_DATACENTER )
							strcat(m_strWindowName, "Datacenter Server " );
						else if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							strcat(m_strWindowName, "Advanced Server " );
						else
							strcat(m_strWindowName, "Server " );
					} else  // Windows NT 4.0 
					{
						if( m_osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							strcat(m_strWindowName, "Server 4.0, Enterprise Edition " );
						else
							strcat(m_strWindowName, "Server 4.0 " );
					}*/
				}
			}/* else  // Test for specific product on Windows NT 4.0 SP5 and earlier
			{
				HKEY hKey;
				char szProductType[80];
				DWORD dwBufLen=80;
				LONG lRet;

				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					"SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
					0, KEY_QUERY_VALUE, &hKey );
				if( lRet != ERROR_SUCCESS )
					return FALSE;

				lRet = RegQueryValueEx( hKey, "ProductType", NULL, NULL,
					(LPBYTE) szProductType, &dwBufLen);
				if( (lRet != ERROR_SUCCESS) || (dwBufLen > 80) )
					return FALSE;

				RegCloseKey( hKey );

				if ( lstrcmpi( "WINNT", szProductType) == 0 )
					strcat(m_strWindowName, "Workstation " );
				if ( lstrcmpi( "LANMANNT", szProductType) == 0 )
					strcat(m_strWindowName, "Server " );
				if ( lstrcmpi( "SERVERNT", szProductType) == 0 )
					strcat(m_strWindowName, "Advanced Server " );

				char strTemp[MAX_PATH];
				sprintf( strTemp, "%d.%d ", m_osvi.dwMajorVersion, m_osvi.dwMinorVersion );
				strcat(m_strWindowName, strTemp);
			}*/

		// Display service pack (if any) and build number.

/*			if( m_osvi.dwMajorVersion == 4 && lstrcmpi( m_osvi.szCSDVersion, "Service Pack 6" ) == 0 )
			{
				HKEY hKey;
				LONG lRet;

				// Test for SP6 versus SP6a.
				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009",
					0, KEY_QUERY_VALUE, &hKey );
				if( lRet == ERROR_SUCCESS )
					sprintf(m_strServiceVersion, "Service Pack 6a (Build %d)\n", m_osvi.dwBuildNumber & 0xFFFF );         
				else // Windows NT 4.0 prior to SP6a
				{
					sprintf(m_strServiceVersion, "%s (Build %d)", m_osvi.szCSDVersion, m_osvi.dwBuildNumber & 0xFFFF);
				}

				RegCloseKey( hKey );
			} else // Windows NT 3.51 and earlier or Windows 2000 and later
			{
				sprintf(m_strServiceVersion, "%s (Build %d)", m_osvi.szCSDVersion, m_osvi.dwBuildNumber & 0xFFFF);
			}*/
			break;

		// Test for the Windows 95 product family.
		case VER_PLATFORM_WIN32_WINDOWS:
/*			if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 0)
			{
				strcpy(m_strWindowName, "Microsoft Windows 95 ");
				if ( m_osvi.szCSDVersion[1] == 'C' || m_osvi.szCSDVersion[1] == 'B' )
				strcpy(m_strServiceVersion, "OSR2 " );
			} */

			if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 10)
			{
				dwVersion = 1;
/*				strcpy(m_strWindowName, "Microsoft Windows 98 ");
				if ( m_osvi.szCSDVersion[1] == 'A' )
				strcpy(m_strServiceVersion, "SE " );*/
			} 

			if (m_osvi.dwMajorVersion == 4 && m_osvi.dwMinorVersion == 90)
			{
				dwVersion = 2;
//				strcpy(m_strWindowName, "Microsoft Windows Millennium Edition");
			} 
			break;
/*
		case VER_PLATFORM_WIN32s:
			strcpy(m_strWindowName, "Microsoft Win32s");
			break;*/
	}

	return dwVersion;
}