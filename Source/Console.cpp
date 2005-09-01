

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
    *                                                                 *
    *   Inbridge Winamp Plugin 2.0                                    *
    *   Copyright © 2005 Sebastian Pipping <webmaster@hartwork.org>   *
    *                                                                 *
    *   --> http://www.hartwork.org                                   *
    *                                                                 *
    *                                                                 *
    *   This source code is released under LGPL.                      *
    *   See LGPL.txt for details.                        2005-09-01   *
    *                                                                 *
    \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "Console.h"
#include <time.h>



#define MENU_INPUT_CONFIG            1
#define MENU_INPUT_ABOUT             2
#define MENU_INPUT_INIT              3
#define MENU_INPUT_QUIT              4
#define MENU_INPUT_GETFILEINFO       5
#define MENU_INPUT_INFOBOX           6
#define MENU_INPUT_ISOURFILE         7
#define MENU_INPUT_PLAY              8
#define MENU_INPUT_PAUSE             9
#define MENU_INPUT_UNPAUSE          10
#define MENU_INPUT_ISPAUSED         11
#define MENU_INPUT_STOP             12
#define MENU_INPUT_GETLENGTH        13
#define MENU_INPUT_GETOUTPUTTIME    14
#define MENU_INPUT_SETOUTPUTTIME    15
#define MENU_INPUT_SETVOLUME        16
#define MENU_INPUT_SETPAN           17
#define MENU_INPUT_EQSET            18

#define MENU_CORE_SAVSAINIT         19
#define MENU_CORE_SAVSADEINIT       20
#define MENU_CORE_SAADDPCMDATA      21
#define MENU_CORE_SAGETMODE         22
#define MENU_CORE_SAADD             23
#define MENU_CORE_VSAADDPCMDATA     24
#define MENU_CORE_VSAGETMODE        25
#define MENU_CORE_VSAADD            26
#define MENU_CORE_VSASETINFO        27
#define MENU_CORE_DSP_ISACTIVE      28
#define MENU_CORE_DSP_DOSAMPLES     29
#define MENU_CORE_SETINFO           30

#define MENU_OUTPUT_CONFIG          31
#define MENU_OUTPUT_ABOUT           32
#define MENU_OUTPUT_INIT            33
#define MENU_OUTPUT_QUIT            34
#define MENU_OUTPUT_OPEN            35
#define MENU_OUTPUT_CLOSE           36
#define MENU_OUTPUT_WRITE           37
#define MENU_OUTPUT_CANWRITE        38
#define MENU_OUTPUT_ISPLAYING       39
#define MENU_OUTPUT_PAUSE           40
#define MENU_OUTPUT_SETVOLUME       41
#define MENU_OUTPUT_SETPAN          42
#define MENU_OUTPUT_FLUSH           43
#define MENU_OUTPUT_GETOUTPUTTIME   45
#define MENU_OUTPUT_GETWRITTENTIME  44



HWND WindowConsole = NULL;

HMENU pupup_menu = NULL;
HMENU input_menu = NULL;
HMENU core_menu = NULL;
HMENU output_menu = NULL;

UINT uNext = 0;
const UINT uMaxEntries = 10000;


WNDPROC WndprocConsoleBackup = NULL;
LRESULT CALLBACK WndprocConsole( HWND hwnd, UINT message, WPARAM wp, LPARAM lp );


BOOL Console::Create( char * szTitle )
{
	WindowConsole = CreateWindowEx(
		WS_EX_TOOLWINDOW |                  // DWORD dwExStyle,
			WS_EX_CLIENTEDGE |              //
			WS_EX_APPWINDOW,                //
		"LISTBOX",                          // LPCTSTR lpClassName
		szTitle,                            // LPCTSTR lpWindowName
		WS_VSCROLL |                        // DWORD dwStyle
			LBS_DISABLENOSCROLL |           //
			LBS_EXTENDEDSEL |               //
			LBS_HASSTRINGS |                //
			LBS_NOTIFY |                    //
			LBS_NOINTEGRALHEIGHT |          //
			WS_VISIBLE |                    //
			WS_POPUP |                      //
			WS_BORDER |                     //
			WS_CAPTION |                    //
			WS_THICKFRAME |                 //
			WS_MAXIMIZEBOX ,                //
		xNormalPosition,                    // int x
		yNormalPosition,                    // int y
		cxNormalPosition,                   // int nWidth
		cyNormalPosition,                   // int nHeight
		NULL,                               // HWND hWndParent
		NULL,                               // HMENU hMenu
		g_hMasterInstance,                  // HINSTANCE hInstance
		NULL                                // LPVOID lpParam
	);
	
	if( !WindowConsole ) return FALSE;

	RECT rcNormalPosition;
	GetWindowRect( WindowConsole, &rcNormalPosition );

	// Window placement
	POINT ptMinPosition = { 0, 0 };
	POINT ptMaxPosition = { xMaxPosition, yMaxPosition };

	WINDOWPLACEMENT placement = {
		sizeof( WINDOWPLACEMENT ),  // UINT length
		0,                          // UINT flags
		( UINT )iShowCmd,           // UINT showCmd
		ptMinPosition,              // POINT ptMinPosition
		ptMaxPosition,              // POINT ptMaxPosition
		rcNormalPosition            // RECT rcNormalPosition
	};

	SetWindowPlacement( WindowConsole, &placement );
	
	
	// So we can always fully read the last line...
	SendMessage( WindowConsole, LB_INSERTSTRING, 0, ( LPARAM )"" );
	
	// Default font
	SendMessage( WindowConsole, WM_SETFONT, ( WPARAM )GetStockObject( DEFAULT_GUI_FONT ), ( LPARAM )TRUE );
	
	
	// Exchange window procedure
	WndprocConsoleBackup = ( WNDPROC )GetWindowLong( WindowConsole, GWL_WNDPROC );
	if( WndprocConsoleBackup != NULL )
	{
		SetWindowLong( WindowConsole, GWL_WNDPROC, ( LONG )WndprocConsole );
	}


	// Create menu
	input_menu = CreatePopupMenu();
	AppendMenu( input_menu, MF_STRING | ( bLogInput_Config            ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_CONFIG,           "Config" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_About             ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_ABOUT,            "About" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_Init              ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_INIT,             "Init" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_Quit              ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_QUIT,             "Quit" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_GetFileInfo       ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_GETFILEINFO,      "GetFileInfo" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_InfoBox           ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_INFOBOX,          "InfoBox" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_IsOurFile         ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_ISOURFILE,        "IsOurFile" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_Play              ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_PLAY,             "Play" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_Pause             ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_PAUSE,            "Pause" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_UnPause           ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_UNPAUSE,          "UnPause" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_IsPaused          ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_ISPAUSED,         "IsPaused" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_Stop              ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_STOP,             "Stop" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_GetLength         ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_GETLENGTH,        "GetLength" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_GetOutputTime     ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_GETOUTPUTTIME,    "GetOutputTime" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_SetOutputTime     ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_SETOUTPUTTIME,    "SetOutputTime" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_SetVolume         ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_SETVOLUME,        "SetVolume" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_SetPan            ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_SETPAN,           "SetPan" );
	AppendMenu( input_menu, MF_STRING | ( bLogInput_EQSet             ? MF_CHECKED : MF_UNCHECKED ), MENU_INPUT_EQSET,            "EQSet" );

	core_menu = CreatePopupMenu();
	AppendMenu( core_menu, MF_STRING | ( bLogCore_SAVSAInit           ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_SAVSAINIT,         "SAVSAInit" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_SAVSADeInit         ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_SAVSADEINIT,       "SAVSADeInit" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_SAAddPCMData        ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_SAADDPCMDATA,      "SAAddPCMData" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_SAGetMode           ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_SAGETMODE,         "SAGetMode" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_SAAdd               ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_SAADD,             "SAAdd" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_VSAAddPCMData       ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_VSAADDPCMDATA,     "VSAAddPCMData" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_VSAGetMode          ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_VSAGETMODE,        "VSAGetMode" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_VSAAdd              ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_VSAADD,            "VSAAdd" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_VSASetInfo          ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_VSASETINFO,        "VSASetInfo" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_dsp_isactive        ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_DSP_ISACTIVE,      "dsp_isactive" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_dsp_dosamples       ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_DSP_DOSAMPLES,     "dsp_dosamples" );
	AppendMenu( core_menu, MF_STRING | ( bLogCore_SetInfo             ? MF_CHECKED : MF_UNCHECKED ), MENU_CORE_SETINFO,           "SetInfo" );

	output_menu = CreatePopupMenu();
	AppendMenu( output_menu, MF_STRING | MF_GRAYED | MF_DISABLED                                   , MENU_OUTPUT_CONFIG,          "Config" );
	AppendMenu( output_menu, MF_STRING | MF_GRAYED | MF_DISABLED                                   , MENU_OUTPUT_ABOUT,           "About" );
	AppendMenu( output_menu, MF_STRING | MF_GRAYED | MF_DISABLED                                   , MENU_OUTPUT_INIT,            "Init" );
	AppendMenu( output_menu, MF_STRING | MF_GRAYED | MF_DISABLED                                   , MENU_OUTPUT_QUIT,            "Quit" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_Open            ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_OPEN,            "Open" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_Close           ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_CLOSE,           "Close" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_Write           ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_WRITE,           "Write" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_CanWrite        ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_CANWRITE,        "CanWrite" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_IsPlaying       ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_ISPLAYING,       "IsPlaying" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_Pause           ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_PAUSE,           "Pause" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_SetVolume       ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_SETVOLUME,       "SetVolume" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_SetPan          ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_SETPAN,          "SetPan" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_Flush           ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_FLUSH,           "Flush" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_GetOutputTime   ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_GETOUTPUTTIME,   "GetOutputTime" );
	AppendMenu( output_menu, MF_STRING | ( bLogOutput_GetWrittenTime  ? MF_CHECKED : MF_UNCHECKED ), MENU_OUTPUT_GETWRITTENTIME,  "GetWrittenTime" );

	pupup_menu = CreatePopupMenu();
	AppendMenu( pupup_menu, MF_STRING | MF_POPUP, ( UINT_PTR )input_menu,   "Input" );
	AppendMenu( pupup_menu, MF_STRING | MF_POPUP, ( UINT_PTR )core_menu,    "Core" );
	AppendMenu( pupup_menu, MF_STRING | MF_POPUP, ( UINT_PTR )output_menu,  "Output" );


	return TRUE;
}


BOOL Console::Destroy()
{
	if( !WindowConsole ) return FALSE;

	// Window placement
	WINDOWPLACEMENT placement = { sizeof( WINDOWPLACEMENT ), };
	GetWindowPlacement( WindowConsole, &placement );

	iShowCmd          = placement.showCmd;
	xMaxPosition      = placement.ptMaxPosition.x;
	yMaxPosition      = placement.ptMaxPosition.y;
	xNormalPosition   = placement.rcNormalPosition.left;
	yNormalPosition   = placement.rcNormalPosition.top;
	cxNormalPosition  = placement.rcNormalPosition.right - placement.rcNormalPosition.left;
	cyNormalPosition  = placement.rcNormalPosition.bottom - placement.rcNormalPosition.top;


	DestroyWindow( WindowConsole );
	DestroyMenu( output_menu );
	DestroyMenu( core_menu );
	DestroyMenu( input_menu );
	DestroyMenu( pupup_menu );

	return TRUE;
}


BOOL Console::Append( char * szText )
{
	if( !WindowConsole ) return FALSE;
	if( uNext > uMaxEntries - 1 )
	{
		SendMessage(
			WindowConsole,
			LB_DELETESTRING,
			0,
			0
		);
		uNext--;
	}
	
	const UINT uTextLen = strlen( szText );
	char * szBuffer = new char[ 11 + uTextLen + 1 ];
	time_t now_time_t = time( NULL );
	struct tm * now_tm = localtime( &now_time_t );
	strftime( szBuffer, 12, "%H:%M:%S   ", now_tm );
	memcpy( szBuffer + 11, szText, uTextLen );
	szBuffer[ 11 + uTextLen ] = TEXT( '\0' );
	
	SendMessage( WindowConsole, LB_INSERTSTRING, uNext, ( LPARAM )szBuffer );
	SendMessage( WindowConsole, LB_SETSEL, FALSE, -1 );
	SendMessage( WindowConsole, LB_SETSEL, TRUE, uNext );
	SendMessage( WindowConsole, LB_SETTOPINDEX, uNext, 0 );
	uNext++;
	
	delete [] szBuffer;
	
	return TRUE;
}


void PopupMenu()
{
	if( !pupup_menu ) return;

	POINT p;
	GetCursorPos( &p );

	BOOL iIndex = TrackPopupMenu(
		pupup_menu,              // HMENU hMenu
		TPM_LEFTALIGN |          // UINT uFlags
			TPM_TOPALIGN |       // .
			TPM_NONOTIFY |       // .
			TPM_RETURNCMD |      // .
			TPM_RIGHTBUTTON,     // .
		p.x,                     // int x
		p.y,                     // int y
		0,                       // int nReserved
		WindowConsole,           // HWND hWnd
		NULL                     // HWND prcRect
	);


	if( iIndex )
	{
		BOOL bChecked = MF_CHECKED & GetMenuState( pupup_menu, iIndex, MF_BYCOMMAND );
		CheckMenuItem( pupup_menu, iIndex, bChecked ? MF_UNCHECKED : MF_CHECKED );
		switch( iIndex )
		{
		case MENU_INPUT_CONFIG:           bLogInput_Config           = !bLogInput_Config;           break;
		case MENU_INPUT_ABOUT:            bLogInput_About            = !bLogInput_About;            break;
		case MENU_INPUT_INIT:             bLogInput_Init             = !bLogInput_Init;             break;
		case MENU_INPUT_QUIT:             bLogInput_Quit             = !bLogInput_Quit;             break;
		case MENU_INPUT_GETFILEINFO:      bLogInput_GetFileInfo      = !bLogInput_GetFileInfo;      break;
		case MENU_INPUT_INFOBOX:          bLogInput_InfoBox          = !bLogInput_InfoBox;          break;
		case MENU_INPUT_ISOURFILE:        bLogInput_IsOurFile        = !bLogInput_IsOurFile;        break;
		case MENU_INPUT_PLAY:             bLogInput_Play             = !bLogInput_Play;             break;
		case MENU_INPUT_PAUSE:            bLogInput_Pause            = !bLogInput_Pause;            break;
		case MENU_INPUT_UNPAUSE:          bLogInput_UnPause          = !bLogInput_UnPause;          break;
		case MENU_INPUT_ISPAUSED:         bLogInput_IsPaused         = !bLogInput_IsPaused;         break;
		case MENU_INPUT_STOP:             bLogInput_Stop             = !bLogInput_Stop;             break;
		case MENU_INPUT_GETLENGTH:        bLogInput_GetLength        = !bLogInput_GetLength;        break;
		case MENU_INPUT_GETOUTPUTTIME:    bLogInput_GetOutputTime    = !bLogInput_GetOutputTime;    break;
		case MENU_INPUT_SETOUTPUTTIME:    bLogInput_SetOutputTime    = !bLogInput_SetOutputTime;    break;
		case MENU_INPUT_SETVOLUME:        bLogInput_SetVolume        = !bLogInput_SetVolume;        break;
		case MENU_INPUT_SETPAN:           bLogInput_SetPan           = !bLogInput_SetPan;           break;
		case MENU_INPUT_EQSET:            bLogInput_EQSet            = !bLogInput_EQSet;            break;

		case MENU_CORE_SAVSAINIT:         bLogCore_SAVSAInit         = !bLogCore_SAVSAInit;         break;
		case MENU_CORE_SAVSADEINIT:       bLogCore_SAVSADeInit       = !bLogCore_SAVSADeInit;       break;
		case MENU_CORE_SAADDPCMDATA:      bLogCore_SAAddPCMData      = !bLogCore_SAAddPCMData;      break;
		case MENU_CORE_SAGETMODE:         bLogCore_SAGetMode         = !bLogCore_SAGetMode;         break;
		case MENU_CORE_SAADD:             bLogCore_SAAdd             = !bLogCore_SAAdd;             break;
		case MENU_CORE_VSAADDPCMDATA:     bLogCore_VSAAddPCMData     = !bLogCore_VSAAddPCMData;     break;
		case MENU_CORE_VSAGETMODE:        bLogCore_VSAGetMode        = !bLogCore_VSAGetMode;        break;
		case MENU_CORE_VSAADD:            bLogCore_VSAAdd            = !bLogCore_VSAAdd;            break;
		case MENU_CORE_VSASETINFO:        bLogCore_VSASetInfo        = !bLogCore_VSASetInfo;        break;
		case MENU_CORE_DSP_ISACTIVE:      bLogCore_dsp_isactive      = !bLogCore_dsp_isactive;      break;
		case MENU_CORE_DSP_DOSAMPLES:     bLogCore_dsp_dosamples     = !bLogCore_dsp_dosamples;     break;
		case MENU_CORE_SETINFO:           bLogCore_SetInfo           = !bLogCore_SetInfo;           break;

//		case MENU_OUTPUT_CONFIG:          bLogOutput_Config          = !bLogOutput_Config;          break;
//		case MENU_OUTPUT_ABOUT:           bLogOutput_About           = !bLogOutput_About;           break;
//		case MENU_OUTPUT_INIT:            bLogOutput_Init            = !bLogOutput_Init;            break;
//		case MENU_OUTPUT_QUIT:            bLogOutput_Quit            = !bLogOutput_Quit;            break;
		case MENU_OUTPUT_OPEN:            bLogOutput_Open            = !bLogOutput_Open;            break;
		case MENU_OUTPUT_CLOSE:           bLogOutput_Close           = !bLogOutput_Close;           break;
		case MENU_OUTPUT_WRITE:           bLogOutput_Write           = !bLogOutput_Write;           break;
		case MENU_OUTPUT_CANWRITE:        bLogOutput_CanWrite        = !bLogOutput_CanWrite;        break;
		case MENU_OUTPUT_ISPLAYING:       bLogOutput_IsPlaying       = !bLogOutput_IsPlaying;       break;
		case MENU_OUTPUT_PAUSE:           bLogOutput_Pause           = !bLogOutput_Pause;           break;
		case MENU_OUTPUT_SETVOLUME:       bLogOutput_SetVolume       = !bLogOutput_SetVolume;       break;
		case MENU_OUTPUT_SETPAN:          bLogOutput_SetPan          = !bLogOutput_SetPan;          break;
		case MENU_OUTPUT_FLUSH:           bLogOutput_Flush           = !bLogOutput_Flush;           break;
		case MENU_OUTPUT_GETOUTPUTTIME:   bLogOutput_GetOutputTime   = !bLogOutput_GetOutputTime;   break;
		case MENU_OUTPUT_GETWRITTENTIME:  bLogOutput_GetWrittenTime  = !bLogOutput_GetWrittenTime;  break;
		}
	}	
}


LRESULT CALLBACK WndprocConsole( HWND hwnd, UINT message, WPARAM wp, LPARAM lp )
{
	static BOOL bContextOpen = FALSE;

	switch( message )
	{
	case WM_CONTEXTMENU:
		{
			if( bContextOpen ) break;
			bContextOpen = TRUE;

			PopupMenu();

			bContextOpen = FALSE;
			break;
		}

	case WM_GETMINMAXINFO:
		{
			MINMAXINFO * mmi = ( MINMAXINFO * )lp;
			mmi->ptMinTrackSize.x = CONSOLE_MIN_WIDTH;
			mmi->ptMinTrackSize.y = CONSOLE_MIN_HEIGHT;
			return 0;
		}
	}
	return CallWindowProc( WndprocConsoleBackup, hwnd, message, wp, lp );
}
