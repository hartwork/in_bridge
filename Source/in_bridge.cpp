

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
    *                                                                 *
    *   Inbridge Winamp Plugin 2.2                                    *
    *   Copyright © 2005 Sebastian Pipping <webmaster@hartwork.org>   *
    *                                                                 *
    *   --> http://www.hartwork.org                                   *
    *                                                                 *
    *                                                                 *
    *   This source code is released under LGPL.                      *
    *   See LGPL.txt for details.                        2006-03-29   *
    *                                                                 *
    \* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "Global.h"
#include "Console.h"
#include "DevilConfig.h"
#include "Winamp/In2.h"
#include "Winamp/Out.h"
#include <stdio.h>



typedef In_Module * ( * WINAMPGETINMODULE2 )();



HINSTANCE g_hMasterInstance = NULL; // extern
HINSTANCE g_hSlaveInstance = NULL;

DEVIL_CONFIG * g_pCONFIG = NULL;

In_Module * g_pInModSlave = NULL;



bool bLogInput_Config; // extern
bool bLogInput_About; // extern
bool bLogInput_Init; // extern
bool bLogInput_Quit; // extern
bool bLogInput_GetFileInfo; // extern
bool bLogInput_InfoBox; // extern
bool bLogInput_IsOurFile; // extern
bool bLogInput_Play; // extern
bool bLogInput_Pause; // extern
bool bLogInput_UnPause; // extern
bool bLogInput_IsPaused; // extern
bool bLogInput_Stop; // extern
bool bLogInput_GetLength; // extern
bool bLogInput_GetOutputTime; // extern
bool bLogInput_SetOutputTime; // extern
bool bLogInput_SetVolume; // extern
bool bLogInput_SetPan; // extern
bool bLogInput_EQSet; // extern

bool bLogCore_SAVSAInit; // extern
bool bLogCore_SAVSADeInit; // extern
bool bLogCore_SAAddPCMData; // extern
bool bLogCore_SAGetMode; // extern
bool bLogCore_SAAdd; // extern
bool bLogCore_VSAAddPCMData; // extern
bool bLogCore_VSAGetMode; // extern
bool bLogCore_VSAAdd; // extern
bool bLogCore_VSASetInfo; // extern
bool bLogCore_dsp_isactive; // extern
bool bLogCore_dsp_dosamples; // extern
bool bLogCore_SetInfo; // extern

// bool bLogOutput_Config; // extern
// bool bLogOutput_About; // extern
// bool bLogOutput_Init; // extern
// bool bLogOutput_Quit; // extern
bool bLogOutput_Open; // extern
bool bLogOutput_Close; // extern
bool bLogOutput_Write; // extern
bool bLogOutput_CanWrite; // extern
bool bLogOutput_IsPlaying; // extern
bool bLogOutput_Pause; // extern
bool bLogOutput_SetVolume; // extern
bool bLogOutput_SetPan; // extern
bool bLogOutput_Flush; // extern
bool bLogOutput_GetOutputTime; // extern
bool bLogOutput_GetWrittenTime; // extern

int iShowCmd; // extern
int xMaxPosition; // extern
int yMaxPosition; // extern
int xNormalPosition; // extern
int yNormalPosition; // extern
int cxNormalPosition; // extern
int cyNormalPosition; // extern



UINT_PTR hMainHandleTimer = 0;
UINT_PTR hRepairTimer = 0;



void  Input_Config( HWND hwndParent );
void  Input_About( HWND hwndParent );
void  Input_Init();
void  Input_Quit();
void  Input_GetFileInfo( char * file, char * title, int * length_in_ms );
int   Input_InfoBox( char * file, HWND hwndParent );
int   Input_IsOurFile( char * fn );
int   Input_Play( char * fn );
void  Input_Pause();
void  Input_UnPause();
int   Input_IsPaused();
void  Input_Stop();
int   Input_GetLength();
int   Input_GetOutputTime();
void  Input_SetOutputTime( int time_in_ms );
void  Input_SetVolume( int volume );
void  Input_SetPan( int pan );
void  Input_EQSet( int on, char data[ 10 ], int preamp );

void  Core_SAVSAInit( int maxlatency_in_ms, int srate );
void  Core_SAVSADeInit();
void  Core_SAAddPCMData( void * PCMData, int nch, int bps, int timestamp );
int   Core_SAGetMode();
void  Core_SAAdd( void * data, int timestamp, int csa );
void  Core_VSAAddPCMData( void * PCMData, int nch, int bps, int timestamp );
int   Core_VSAGetMode( int * specNch, int * waveNch );
void  Core_VSAAdd( void * data, int timestamp );
void  Core_VSASetInfo( int srate, int nch );
int   Core_dsp_isactive();
int   Core_dsp_dosamples( short int * samples, int numsamples, int bps, int nch, int srate );
void  Core_SetInfo( int bitrate, int srate, int stereo, int synched );

// void  Output_Config( HWND p );
// void  Output_About( HWND p );
// void  Output_Init();
// void  Output_Quit();
int   Output_Open( int sr, int nch, int bps, int bufferlenms, int prebufferms );
void  Output_Close();
int   Output_Write( char * data, int size );
int   Output_CanWrite();
int   Output_IsPlaying();
int   Output_Pause( int new_state );
void  Output_SetVolume( int v );
void  Output_SetPan( int p );
void  Output_Flush( int pos );
int   Output_GetOutputTime();
int   Output_GetWrittenTime();



////////////////////////////////////////////////////////////////////////////////
//  GetCurrentModule
////////////////////////////////////////////////////////////////////////////////
HMODULE GetCurrentModule()
{
	MEMORY_BASIC_INFORMATION mbi;
	static int dummy;
	VirtualQuery( &dummy, &mbi, sizeof( MEMORY_BASIC_INFORMATION ) );
	return ( HMODULE )mbi.AllocationBase;
}



////////////////////////////////////////////////////////////////////////////////
//  ReadConfig
////////////////////////////////////////////////////////////////////////////////
void ReadConfig()
{
	if( !g_pCONFIG ) return;

	int res;

	g_pCONFIG->Read( "bLogInput_Config",           &res, 1 ); bLogInput_Config           = ( bool )res;
	g_pCONFIG->Read( "bLogInput_About",            &res, 1 ); bLogInput_About            = ( bool )res;
	g_pCONFIG->Read( "bLogInput_Init",             &res, 1 ); bLogInput_Init             = ( bool )res;
	g_pCONFIG->Read( "bLogInput_Quit",             &res, 1 ); bLogInput_Quit             = ( bool )res;
	g_pCONFIG->Read( "bLogInput_GetFileInfo",      &res, 1 ); bLogInput_GetFileInfo      = ( bool )res;
	g_pCONFIG->Read( "bLogInput_InfoBox",          &res, 1 ); bLogInput_InfoBox          = ( bool )res;
	g_pCONFIG->Read( "bLogInput_IsOurFile",        &res, 1 ); bLogInput_IsOurFile        = ( bool )res;
	g_pCONFIG->Read( "bLogInput_Play",             &res, 1 ); bLogInput_Play             = ( bool )res;
	g_pCONFIG->Read( "bLogInput_Pause",            &res, 1 ); bLogInput_Pause            = ( bool )res;
	g_pCONFIG->Read( "bLogInput_UnPause",          &res, 1 ); bLogInput_UnPause          = ( bool )res;
	g_pCONFIG->Read( "bLogInput_IsPaused",         &res, 1 ); bLogInput_IsPaused         = ( bool )res;
	g_pCONFIG->Read( "bLogInput_Stop",             &res, 1 ); bLogInput_Stop             = ( bool )res;
	g_pCONFIG->Read( "bLogInput_GetLength",        &res, 0 ); bLogInput_GetLength        = ( bool )res;
	g_pCONFIG->Read( "bLogInput_GetOutputTime",    &res, 0 ); bLogInput_GetOutputTime    = ( bool )res;
	g_pCONFIG->Read( "bLogInput_SetOutputTime",    &res, 1 ); bLogInput_SetOutputTime    = ( bool )res;
	g_pCONFIG->Read( "bLogInput_SetVolume",        &res, 1 ); bLogInput_SetVolume        = ( bool )res;
	g_pCONFIG->Read( "bLogInput_SetPan",           &res, 1 ); bLogInput_SetPan           = ( bool )res;
	g_pCONFIG->Read( "bLogInput_EQSet",            &res, 1 ); bLogInput_EQSet            = ( bool )res;

	g_pCONFIG->Read( "bLogCore_SAVSAInit",         &res, 1 ); bLogCore_SAVSAInit         = ( bool )res;
	g_pCONFIG->Read( "bLogCore_SAVSADeInit",       &res, 1 ); bLogCore_SAVSADeInit       = ( bool )res;
	g_pCONFIG->Read( "bLogCore_SAAddPCMData",      &res, 0 ); bLogCore_SAAddPCMData      = ( bool )res;
	g_pCONFIG->Read( "bLogCore_SAGetMode",         &res, 0 ); bLogCore_SAGetMode         = ( bool )res;
	g_pCONFIG->Read( "bLogCore_SAAdd",             &res, 0 ); bLogCore_SAAdd             = ( bool )res;
	g_pCONFIG->Read( "bLogCore_VSAAddPCMData",     &res, 0 ); bLogCore_VSAAddPCMData     = ( bool )res;
	g_pCONFIG->Read( "bLogCore_VSAGetMode",        &res, 0 ); bLogCore_VSAGetMode        = ( bool )res;
	g_pCONFIG->Read( "bLogCore_VSAAdd",            &res, 0 ); bLogCore_VSAAdd            = ( bool )res;
	g_pCONFIG->Read( "bLogCore_VSASetInfo",        &res, 1 ); bLogCore_VSASetInfo        = ( bool )res;
	g_pCONFIG->Read( "bLogCore_dsp_isactive",      &res, 0 ); bLogCore_dsp_isactive      = ( bool )res;
	g_pCONFIG->Read( "bLogCore_dsp_dosamples",     &res, 0 ); bLogCore_dsp_dosamples     = ( bool )res;
	g_pCONFIG->Read( "bLogCore_SetInfo",           &res, 0 ); bLogCore_SetInfo           = ( bool )res;

// 	g_pCONFIG->Read( "bLogOutput_Config",          &res, 1 ); bLogOutput_Config          = ( bool )res;
// 	g_pCONFIG->Read( "bLogOutput_About",           &res, 1 ); bLogOutput_About           = ( bool )res;
// 	g_pCONFIG->Read( "bLogOutput_Init",            &res, 1 ); bLogOutput_Init            = ( bool )res;
// 	g_pCONFIG->Read( "bLogOutput_Quit",            &res, 1 ); bLogOutput_Quit            = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_Open",            &res, 1 ); bLogOutput_Open            = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_Close",           &res, 1 ); bLogOutput_Close           = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_Write",           &res, 0 ); bLogOutput_Write           = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_CanWrite",        &res, 0 ); bLogOutput_CanWrite        = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_IsPlaying",       &res, 1 ); bLogOutput_IsPlaying       = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_Pause",           &res, 1 ); bLogOutput_Pause           = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_SetVolume",       &res, 1 ); bLogOutput_SetVolume       = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_SetPan",          &res, 1 ); bLogOutput_SetPan          = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_Flush",           &res, 1 ); bLogOutput_Flush           = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_GetOutputTime",   &res, 0 ); bLogOutput_GetOutputTime   = ( bool )res;
	g_pCONFIG->Read( "bLogOutput_GetWrittenTime",  &res, 0 ); bLogOutput_GetWrittenTime  = ( bool )res;


	const int cxScreen = GetSystemMetrics( SM_CXFULLSCREEN );
	const int cyScreen = GetSystemMetrics( SM_CYFULLSCREEN );

	g_pCONFIG->Read( "iShowCmd",           &iShowCmd,          SW_SHOWNORMAL );
	g_pCONFIG->Read( "xMaxPosition",       &xMaxPosition,      -1 );
	g_pCONFIG->Read( "yMaxPosition",       &yMaxPosition,      -1 );
	g_pCONFIG->Read( "xNormalPosition",    &xNormalPosition,   ( cxScreen - CONSOLE_DEFAULT_WIDTH ) / 2 );
	g_pCONFIG->Read( "yNormalPosition",    &yNormalPosition,   ( cyScreen - CONSOLE_DEFAULT_HEIGHT ) / 2 );
	g_pCONFIG->Read( "cxNormalPosition",   &cxNormalPosition,  CONSOLE_DEFAULT_WIDTH );
	g_pCONFIG->Read( "cyNormalPosition",   &cyNormalPosition,  CONSOLE_DEFAULT_HEIGHT );
}



////////////////////////////////////////////////////////////////////////////////
//  WriteConfig
////////////////////////////////////////////////////////////////////////////////
void WriteConfig()
{
	if( !g_pCONFIG ) return;

	g_pCONFIG->Write( "bLogInput_Config",           ( int )bLogInput_Config );
	g_pCONFIG->Write( "bLogInput_About",            ( int )bLogInput_About );
	g_pCONFIG->Write( "bLogInput_Init",             ( int )bLogInput_Init );
	g_pCONFIG->Write( "bLogInput_Quit",             ( int )bLogInput_Quit );
	g_pCONFIG->Write( "bLogInput_GetFileInfo",      ( int )bLogInput_GetFileInfo );
	g_pCONFIG->Write( "bLogInput_InfoBox",          ( int )bLogInput_InfoBox );
	g_pCONFIG->Write( "bLogInput_IsOurFile",        ( int )bLogInput_IsOurFile );
	g_pCONFIG->Write( "bLogInput_Play",             ( int )bLogInput_Play );
	g_pCONFIG->Write( "bLogInput_Pause",            ( int )bLogInput_Pause );
	g_pCONFIG->Write( "bLogInput_UnPause",          ( int )bLogInput_UnPause );
	g_pCONFIG->Write( "bLogInput_IsPaused",         ( int )bLogInput_IsPaused );
	g_pCONFIG->Write( "bLogInput_Stop",             ( int )bLogInput_Stop );
	g_pCONFIG->Write( "bLogInput_GetLength",        ( int )bLogInput_GetLength );
	g_pCONFIG->Write( "bLogInput_GetOutputTime",    ( int )bLogInput_GetOutputTime );
	g_pCONFIG->Write( "bLogInput_SetOutputTime",    ( int )bLogInput_SetOutputTime );
	g_pCONFIG->Write( "bLogInput_SetVolume",        ( int )bLogInput_SetVolume );
	g_pCONFIG->Write( "bLogInput_SetPan",           ( int )bLogInput_SetPan );
	g_pCONFIG->Write( "bLogInput_EQSet",            ( int )bLogInput_EQSet );
	
	g_pCONFIG->Write( "bLogCore_SAVSAInit",         ( int )bLogCore_SAVSAInit );
	g_pCONFIG->Write( "bLogCore_SAVSADeInit",       ( int )bLogCore_SAVSADeInit );
	g_pCONFIG->Write( "bLogCore_SAAddPCMData",      ( int )bLogCore_SAAddPCMData );
	g_pCONFIG->Write( "bLogCore_SAGetMode",         ( int )bLogCore_SAGetMode );
	g_pCONFIG->Write( "bLogCore_SAAdd",             ( int )bLogCore_SAAdd );
	g_pCONFIG->Write( "bLogCore_VSAAddPCMData",     ( int )bLogCore_VSAAddPCMData );
	g_pCONFIG->Write( "bLogCore_VSAGetMode",        ( int )bLogCore_VSAGetMode );
	g_pCONFIG->Write( "bLogCore_VSAAdd",            ( int )bLogCore_VSAAdd );
	g_pCONFIG->Write( "bLogCore_VSASetInfo",        ( int )bLogCore_VSASetInfo );
	g_pCONFIG->Write( "bLogCore_dsp_isactive",      ( int )bLogCore_dsp_isactive );
	g_pCONFIG->Write( "bLogCore_dsp_dosamples",     ( int )bLogCore_dsp_dosamples );
	g_pCONFIG->Write( "bLogCore_SetInfo",           ( int )bLogCore_SetInfo );
	
// 	g_pCONFIG->Write( "bLogOutput_Config",          ( int )bLogOutput_Config );
// 	g_pCONFIG->Write( "bLogOutput_About",           ( int )bLogOutput_About );
// 	g_pCONFIG->Write( "bLogOutput_Init",            ( int )bLogOutput_Init );
// 	g_pCONFIG->Write( "bLogOutput_Quit",            ( int )bLogOutput_Quit );
	g_pCONFIG->Write( "bLogOutput_Open",            ( int )bLogOutput_Open );
	g_pCONFIG->Write( "bLogOutput_Close",           ( int )bLogOutput_Close );
	g_pCONFIG->Write( "bLogOutput_Write",           ( int )bLogOutput_Write );
	g_pCONFIG->Write( "bLogOutput_CanWrite",        ( int )bLogOutput_CanWrite );
	g_pCONFIG->Write( "bLogOutput_IsPlaying",       ( int )bLogOutput_IsPlaying );
	g_pCONFIG->Write( "bLogOutput_Pause",           ( int )bLogOutput_Pause );
	g_pCONFIG->Write( "bLogOutput_SetVolume",       ( int )bLogOutput_SetVolume );
	g_pCONFIG->Write( "bLogOutput_SetPan",          ( int )bLogOutput_SetPan );
	g_pCONFIG->Write( "bLogOutput_Flush",           ( int )bLogOutput_Flush );
	g_pCONFIG->Write( "bLogOutput_GetOutputTime",   ( int )bLogOutput_GetOutputTime );
	g_pCONFIG->Write( "bLogOutput_GetWrittenTime",  ( int )bLogOutput_GetWrittenTime );


	g_pCONFIG->Write( "iShowCmd",          iShowCmd );
	g_pCONFIG->Write( "xMaxPosition",      xMaxPosition );
	g_pCONFIG->Write( "yMaxPosition",      yMaxPosition );
	g_pCONFIG->Write( "xNormalPosition",   xNormalPosition );
	g_pCONFIG->Write( "yNormalPosition",   yNormalPosition );
	g_pCONFIG->Write( "cxNormalPosition",  cxNormalPosition );
	g_pCONFIG->Write( "cyNormalPosition",  cyNormalPosition );
}



////////////////////////////////////////////////////////////////////////////////
//  g_InModMaster
////////////////////////////////////////////////////////////////////////////////
In_Module g_InModMaster = {
	IN_VER,                    // int version
	NULL,                      // char * description
	    NULL,                  // HWND hMainWindow
	    NULL,                  // HINSTANCE hDllInstance
	NULL,                      // char * FileExtensions
	1,                         // int is_seekable
	1,                         // int UsesOutputPlug
	Input_Config,              // void ( * Config )( HWND hwndParent )
	Input_About,               // void ( * About )( HWND hwndParent )
	Input_Init,                // void ( * Init )()
	Input_Quit,                // void ( * Quit )()
	Input_GetFileInfo,         // void ( * GetFileInfo )( char * file, char * title, int * length_in_ms )
	Input_InfoBox,             // int ( * InfoBox )( char * file, HWND hwndParent )
	Input_IsOurFile,           // int ( * IsOurFile )( char * fn )
	Input_Play,                // int ( * Play )( char * fn )
	Input_Pause,               // void ( * Pause )()
	Input_UnPause,             // void ( * UnPause )()
	Input_IsPaused,            // int ( * IsPaused )()
	Input_Stop,                // void ( * Stop )()
	Input_GetLength,           // int ( * GetLength )()
	Input_GetOutputTime,       // int ( * GetOutputTime )()
	Input_SetOutputTime,       // void ( * SetOutputTime )( int time_in_ms )
	Input_SetVolume,           // void ( * SetVolume )( int volume )
	Input_SetPan,              // void ( * SetPan )( int pan )
	    NULL,                  // void ( * SAVSAInit )( int maxlatency_in_ms, int srate )
	    NULL,                  // void ( * SAVSADeInit )()
	    NULL,                  // void ( * SAAddPCMData )( void * PCMData, int nch, int bps, int timestamp )
	    NULL,                  // int ( * SAGetMode )()
	    NULL,                  // void ( * SAAdd )( void * data, int timestamp, int csa )
	    NULL,                  // void ( * VSAAddPCMData )( void * PCMData, int nch, int bps, int timestamp )
	    NULL,                  // int ( * VSAGetMode )( int * specNch, int * waveNch )
	    NULL,                  // void ( * VSAAdd )( void * data, int timestamp )
	    NULL,                  // void ( * VSASetInfo )( int srate, int nch )
	    NULL,                  // int ( * dsp_isactive )()
	    NULL,                  // int ( * dsp_dosamples )( short int * samples, int numsamples, int bps, int nch, int srate )
	Input_EQSet,               // void ( * EQSet )( int on, char data[ 10 ], int preamp )
	    NULL,                  // void ( * SetInfo )( int bitrate, int srate, int stereo, int synched )
	    NULL                   // Out_Module * outMod
};



////////////////////////////////////////////////////////////////////////////////
//  g_OutModMaster
////////////////////////////////////////////////////////////////////////////////
Out_Module g_OutModMaster = {
	OUT_VER,                    // int version
	NULL,                       // char * description
	0,                          // int id
		0,                      // HWND hMainWindow
		0,                      // HINSTANCE hDllInstance
	NULL,                       // void ( * Config )( HWND p )
	NULL,                       // void ( * About )( HWND p )
	NULL,                       // void ( * Init )()
	NULL,                       // void ( * Quit )()
	Output_Open,                // int ( * Open )( int sr, int nch, int bps, int bufferlenms, int prebufferms )
	Output_Close,               // void ( * Close )()
	Output_Write,               // int ( * Write )( char * data, int size )
	Output_CanWrite,            // int ( * CanWrite )()
	Output_IsPlaying,           // int ( * IsPlaying )()
	Output_Pause,               // int ( * Pause )( int new_state )
	Output_SetVolume,           // void ( * SetVolume )( int v )
	Output_SetPan,              // void ( * SetPan )( int p )
	Output_Flush,               // void ( * Flush )( int pos )
	Output_GetOutputTime,       // int ( * GetOutputTime )()
	Output_GetWrittenTime       // int ( * GetWrittenTime )()
};



////////////////////////////////////////////////////////////////////////////////
//  Input_Config
////////////////////////////////////////////////////////////////////////////////
void Input_Config( HWND hwndParent )
{
	if( bLogInput_Config )
	{
		Console::Append( "Input::Config( HWND hwndParent )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   hwndParent = %i", ( int )hwndParent );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_pInModSlave->Config( hwndParent );
	
	// Apply possibly changed extension set
	g_InModMaster.FileExtensions = g_pInModSlave->FileExtensions;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_About
////////////////////////////////////////////////////////////////////////////////
void Input_About( HWND hwndParent )
{
	if( bLogInput_About )
	{
		Console::Append( "Input::About( HWND hwndParent )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   hwndParent = %i", ( int )hwndParent );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_pInModSlave->About( hwndParent );
}



////////////////////////////////////////////////////////////////////////////////
//  Input_Init
////////////////////////////////////////////////////////////////////////////////
void Input_Init()
{
	if( bLogInput_Init )
	{
		Console::Append( "Input::Init()" );
		Console::Append( " " );
	}

	g_pInModSlave->Init();

	// Enable core functions to slave
	g_pInModSlave->SAVSAInit      = Core_SAVSAInit;
	g_pInModSlave->SAVSADeInit    = Core_SAVSADeInit;
	g_pInModSlave->SAAddPCMData   = Core_SAAddPCMData;
	g_pInModSlave->SAGetMode      = Core_SAGetMode;
	g_pInModSlave->SAAdd          = Core_SAAdd;
	g_pInModSlave->VSAAddPCMData  = Core_VSAAddPCMData;
	g_pInModSlave->VSAGetMode     = Core_VSAGetMode;
	g_pInModSlave->VSAAdd         = Core_VSAAdd;
	g_pInModSlave->VSASetInfo     = Core_VSASetInfo;
	g_pInModSlave->dsp_isactive   = Core_dsp_isactive;
	g_pInModSlave->dsp_dosamples  = Core_dsp_dosamples;
	g_pInModSlave->SetInfo        = Core_SetInfo;
	
	// Modifiy master
	g_InModMaster.description     = g_pInModSlave->description;
	g_InModMaster.FileExtensions  = g_pInModSlave->FileExtensions;
	g_InModMaster.is_seekable     = g_pInModSlave->is_seekable;
	g_InModMaster.UsesOutputPlug  = g_pInModSlave->UsesOutputPlug;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_Quit
////////////////////////////////////////////////////////////////////////////////
void Input_Quit()
{
	// In case Quit() is called right after Init()
	// which can happen when Plainamp scans the plugin
	// the timer is still running which means we are
	// unloading the plugin although is still running.
	// Bad idea so we have to stop the timer.
	if( hMainHandleTimer )
	{
		KillTimer( NULL, hMainHandleTimer );
		hMainHandleTimer = 0;
	}

	if( bLogInput_Quit )
	{
		Console::Append( "Input::Quit()" );
		Console::Append( " " );
	}
	
	g_pInModSlave->Quit();
	
	// Unload
	FreeLibrary( g_hSlaveInstance );

	// Stop repair "thread"
	KillTimer( NULL, hRepairTimer );
	hRepairTimer = 0;
	
	// Destroy console
	Console::Destroy();
	
	// Write config
	WriteConfig();
}



////////////////////////////////////////////////////////////////////////////////
//  Input_GetFileInfo
////////////////////////////////////////////////////////////////////////////////
void Input_GetFileInfo( char * file, char * title, int * length_in_ms )
{
	if( bLogInput_GetFileInfo )
	{
		Console::Append( "Input::GetFileInfo( char * file, char * title, int * length_in_ms )" );
		if( file )
		{
			char szBuffer[ 500 ];
			sprintf( szBuffer, "   file = \"%s\"", file );
			Console::Append( szBuffer );
		}
		else
		{
			Console::Append( "   file = NULL" );
		}
	}
	
	g_pInModSlave->GetFileInfo( file, title, length_in_ms );
	
	// Update seek flag	
	g_InModMaster.is_seekable = g_pInModSlave->is_seekable;

	if( bLogInput_GetFileInfo )
	{
		char szBuffer[ 500 ];
		if( title )
		{
			sprintf( szBuffer, "   title = \"%s\"", title );
			Console::Append( szBuffer );
		}
		else
		{
			Console::Append( "   title = NULL" );
		}

		if( length_in_ms )
		{
			sprintf( szBuffer, "   length_in_ms = %i", *length_in_ms );
			Console::Append( szBuffer );
		}
		else
		{
			Console::Append( "   length_in_ms = NULL" );
		}
		Console::Append( " " );
	}
}



////////////////////////////////////////////////////////////////////////////////
//  Input_InfoBox
////////////////////////////////////////////////////////////////////////////////
int Input_InfoBox( char * file, HWND hwndParent )
{
	if( bLogInput_InfoBox )
	{
		Console::Append( "Input::InfoBox( char * file, HWND hwndParent )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   file = \"%s\"", ( file ? file : "NULL" ) );
		Console::Append( szBuffer );
	}

	int res = g_pInModSlave->InfoBox( file, hwndParent );
	
	if( bLogInput_InfoBox )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_IsOurFile
////////////////////////////////////////////////////////////////////////////////
int Input_IsOurFile( char * fn )
{
	if( bLogInput_IsOurFile )
	{
		Console::Append( "Input::IsOurFile( char * fn )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   fn = \"%s\"", ( fn ? fn : "NULL" ) );
		Console::Append( szBuffer );
	}

	int res = g_pInModSlave->IsOurFile( fn );
	
	if( bLogInput_IsOurFile )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_Play
////////////////////////////////////////////////////////////////////////////////
int Input_Play( char * fn )
{
	if( bLogInput_Play )
	{
		Console::Append( "Input::Play( char * fn )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   fn = \"%s\"", ( fn ? fn : "NULL" ) );
		Console::Append( szBuffer );
	}

	int res = g_pInModSlave->Play( fn );
	
	if( bLogInput_Play )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_Pause
////////////////////////////////////////////////////////////////////////////////
void Input_Pause()
{
	if( bLogInput_Pause )
	{
		Console::Append( "Input::Pause()" );
		Console::Append( " " );
	}

	g_pInModSlave->Pause();
}



////////////////////////////////////////////////////////////////////////////////
//  Input_UnPause
////////////////////////////////////////////////////////////////////////////////
void Input_UnPause()
{
	if( bLogInput_UnPause )
	{
		Console::Append( "Input::UnPause()" );
		Console::Append( " " );
	}

	g_pInModSlave->UnPause();
}



////////////////////////////////////////////////////////////////////////////////
//  Input_IsPaused
////////////////////////////////////////////////////////////////////////////////
int Input_IsPaused()
{
	if( bLogInput_IsPaused )
	{
		Console::Append( "Input::IsPaused()" );
	}

	int res = g_pInModSlave->IsPaused();
	
	if( bLogInput_IsPaused )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_Stop
////////////////////////////////////////////////////////////////////////////////
void Input_Stop()
{
	if( bLogInput_Stop )
	{
		Console::Append( "Input::Stop()" );
		Console::Append( " " );
	}

	g_pInModSlave->Stop();
}



////////////////////////////////////////////////////////////////////////////////
//  Input_GetLength
////////////////////////////////////////////////////////////////////////////////
int Input_GetLength()
{
	if( bLogInput_GetLength )
	{
		Console::Append( "Input::GetLength()" );
	}

	int res = g_pInModSlave->GetLength();
	
	if( bLogInput_GetLength )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_GetOutputTime
////////////////////////////////////////////////////////////////////////////////
int Input_GetOutputTime()
{
	if( bLogInput_GetOutputTime )
	{
		Console::Append( "Input::GetOutputTime()" );
	}

	int res = g_pInModSlave->GetOutputTime();

	if( bLogInput_GetOutputTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Input_SetOutputTime
////////////////////////////////////////////////////////////////////////////////
void Input_SetOutputTime( int time_in_ms )
{
	if( bLogInput_SetOutputTime )
	{
		Console::Append( "Input::SetOutputTime( int time_in_ms )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   time_in_ms = %i", time_in_ms );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_pInModSlave->SetOutputTime( time_in_ms );
}



////////////////////////////////////////////////////////////////////////////////
//  Input_SetVolume
////////////////////////////////////////////////////////////////////////////////
void Input_SetVolume( int volume )
{
	if( bLogInput_SetVolume )
	{
		Console::Append( "Input::SetVolume( int volume )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   volume = %i", volume );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_pInModSlave->SetVolume( volume );
}



////////////////////////////////////////////////////////////////////////////////
//  Input_SetPan
////////////////////////////////////////////////////////////////////////////////
void Input_SetPan( int pan )
{
	if( bLogInput_SetPan )
	{
		Console::Append( "Input::SetPan( int pan )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   pan = %i", pan );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_pInModSlave->SetPan( pan );
}



////////////////////////////////////////////////////////////////////////////////
//  Input_EQSet
////////////////////////////////////////////////////////////////////////////////
void Input_EQSet( int on, char data[ 10 ], int preamp )
{
	if( bLogInput_SetPan )
	{
		Console::Append( "Input::EQSet( int on, char data[ 10 ], int preamp )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   on = %i", on );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   data: [%i, %i, %i, %i, %i, %i, %i, %i, %i, %i]",
			data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ], data[ 4 ],
			data[ 5 ], data[ 6 ], data[ 7 ], data[ 8 ], data[ 9 ] );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   preamp = %i", preamp );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_pInModSlave->EQSet( on, data, preamp );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_SAVSAInit
////////////////////////////////////////////////////////////////////////////////
void Core_SAVSAInit( int maxlatency_in_ms, int srate )
{
	if( bLogCore_SAVSAInit )
	{
		Console::Append( "Core::SAVSAInit( int maxlatency_in_ms, int srate )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   maxlatency_in_ms = %i", maxlatency_in_ms );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   srate = %i", srate );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.SAVSAInit( maxlatency_in_ms, srate );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_SAVSADeInit
////////////////////////////////////////////////////////////////////////////////
void Core_SAVSADeInit()
{
	if( bLogCore_SAVSADeInit )
	{
		Console::Append( "Core::SAVSADeInit()" );
		Console::Append( " " );
	}

	g_InModMaster.SAVSADeInit();
}



////////////////////////////////////////////////////////////////////////////////
//  Core_SAAddPCMData
////////////////////////////////////////////////////////////////////////////////
void Core_SAAddPCMData( void * PCMData, int nch, int bps, int timestamp )
{
	if( bLogCore_SAAddPCMData )
	{
		Console::Append( "Core::SAAddPCMData( void * PCMData, int nch, int bps, int timestamp )" );
		Console::Append( "   PCMData = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   nch = %i", nch );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   bps = %i", bps );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   timestamp = %i", timestamp );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.SAAddPCMData( PCMData, nch, bps, timestamp );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_SAGetMode
////////////////////////////////////////////////////////////////////////////////
int Core_SAGetMode()
{
	if( bLogCore_SAGetMode )
	{
		Console::Append( "Core::SAGetMode()" );
	}

	int res = g_InModMaster.SAGetMode();

	if( bLogCore_SAGetMode )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Core_SAAdd
////////////////////////////////////////////////////////////////////////////////
void Core_SAAdd( void * data, int timestamp, int csa )
{
	if( bLogCore_SAAdd )
	{
		Console::Append( "Core::SAAdd( void * data, int timestamp, int csa )" );
		Console::Append( "   data = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   timestamp = %i", timestamp );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   csa = %i", csa );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.SAAdd( data, timestamp, csa );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_VSAAddPCMData
////////////////////////////////////////////////////////////////////////////////
void Core_VSAAddPCMData( void * PCMData, int nch, int bps, int timestamp )
{
	if( bLogCore_VSAAddPCMData )
	{
		Console::Append( "Core::VSAAddPCMData( void * PCMData, int nch, int bps, int timestamp )" );
		Console::Append( "   PCMData = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   nch = %i", nch );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   bps = %i", bps );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   timestamp = %i", timestamp );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.VSAAddPCMData( PCMData, nch, bps, timestamp );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_VSAGetMode
////////////////////////////////////////////////////////////////////////////////
int Core_VSAGetMode( int * specNch, int * waveNch )
{
	if( bLogCore_VSAGetMode )
	{
		Console::Append( "Core::VSAGetMode( int * specNch, int * waveNch )" );
	}

	int res = g_InModMaster.VSAGetMode( specNch, waveNch );

	if( bLogCore_VSAGetMode )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   *specNch = %i", ( specNch ? *specNch : -1 ) );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   *waveNch = %i", ( waveNch ? *waveNch : -1 ) );
		Console::Append( szBuffer );
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}	
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Core_VSAAdd
////////////////////////////////////////////////////////////////////////////////
void Core_VSAAdd( void * data, int timestamp )
{
	if( bLogCore_VSAAdd )
	{
		Console::Append( "Core::VSAAdd( void * data, int timestamp )" );
		Console::Append( "   data = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   timestamp = %i", timestamp );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.VSAAdd( data, timestamp );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_VSASetInfo
////////////////////////////////////////////////////////////////////////////////
void Core_VSASetInfo( int srate, int nch )
{
	if( bLogCore_VSASetInfo )
	{
		Console::Append( "Core::VSASetInfo( int srate, int nch )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   srate = %i", srate );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   nch = %i", nch );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.VSASetInfo( srate, nch );
}



////////////////////////////////////////////////////////////////////////////////
//  Core_dsp_isactive
////////////////////////////////////////////////////////////////////////////////
int Core_dsp_isactive()
{
	if( bLogCore_dsp_isactive )
	{
		Console::Append( "Core::dsp_isactive()" );
	}

	int res = g_InModMaster.dsp_isactive();
	
	if( bLogCore_dsp_isactive )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Core_dsp_dosamples
////////////////////////////////////////////////////////////////////////////////
int Core_dsp_dosamples( short int * samples, int numsamples, int bps, int nch, int srate )
{
	if( bLogCore_dsp_dosamples )
	{
		Console::Append( "Core::dsp_dosamples( short int * samples, int numsamples, int bps, int nch, int srate )" );
		Console::Append( "   samples = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   numsamples = %i", numsamples );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   bps = %i", bps );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   nch = %i", nch );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   srate = %i", srate );
		Console::Append( szBuffer );
	}

	int res = g_InModMaster.dsp_dosamples( samples, numsamples, bps, nch, srate );
	
	if( bLogCore_dsp_dosamples )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Core_SetInfo
////////////////////////////////////////////////////////////////////////////////
void Core_SetInfo( int bitrate, int srate, int stereo, int synched )
{
	if( bLogCore_SetInfo )
	{
		Console::Append( "Core::SetInfo( int bitrate, int srate, int stereo, int synched )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   bitrate = %i", bitrate );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   srate = %i", srate );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   stereo = %i", stereo );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   synched = %i", synched );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.SetInfo( bitrate, srate, stereo, synched );
}


/*
////////////////////////////////////////////////////////////////////////////////
//  Output_Config
////////////////////////////////////////////////////////////////////////////////
void Output_Config( HWND p )
{
	if( bLogOutput_Config )
	{
		Console::Append( "Output::Config( HWND p )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   p = %i", ( int )p );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_InModMaster.outMod->Config( p );
}



////////////////////////////////////////////////////////////////////////////////
///  Output_About
////////////////////////////////////////////////////////////////////////////////
void Output_About( HWND p )
{
	if( bLogOutput_About )
	{
		Console::Append( "Output::About( HWND p )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   p = %i", ( int )p );
		Console::Append( szBuffer );
		Console::Append( " " );
	}

	g_InModMaster.outMod->About( p );
}



////////////////////////////////////////////////////////////////////////////////
//  Output_Init
////////////////////////////////////////////////////////////////////////////////
void Output_Init()
{
	if( bLogOutput_Init )
	{
		Console::Append( "Output::Init()" );
		Console::Append( " " );
	}

	g_InModMaster.outMod->Init();
}



////////////////////////////////////////////////////////////////////////////////
//  Output_Quit
////////////////////////////////////////////////////////////////////////////////
void Output_Quit()
{
	if( bLogOutput_Quit )
	{
		Console::Append( "Output::Quit()" );
		Console::Append( " " );
	}

	g_InModMaster.outMod->Quit();
}
*/


////////////////////////////////////////////////////////////////////////////////
//  Output_Open
////////////////////////////////////////////////////////////////////////////////
int Output_Open( int sr, int nch, int bps, int bufferlenms, int prebufferms )
{
	if( bLogOutput_Open )
	{
		Console::Append( "Output::Open( int sr, int nch, int bps, int bufferlenms, int prebufferms )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   sr = %i", sr );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   nch = %i", nch );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   bps = %i", bps );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   bufferlenms = %i", bufferlenms );
		Console::Append( szBuffer );
		sprintf( szBuffer, "   prebufferms = %i", prebufferms );
		Console::Append( szBuffer );
	}

	int res = g_InModMaster.outMod->Open( sr, nch, bps, bufferlenms, prebufferms );
	
	if( bLogOutput_Open )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Output_Close
////////////////////////////////////////////////////////////////////////////////
void Output_Close()
{
	if( bLogOutput_Close )
	{
		Console::Append( "Output::Close()" );
		Console::Append( " " );
	}

	g_InModMaster.outMod->Close();
}



////////////////////////////////////////////////////////////////////////////////
//  Output_Write
////////////////////////////////////////////////////////////////////////////////
int Output_Write( char * data, int size )
{
	if( bLogOutput_Write )
	{
		Console::Append( "Output::Write( char * data, int size )" );
		Console::Append( "   data = ..." );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   size = %i", size );
		Console::Append( szBuffer );
	}

	int res = g_InModMaster.outMod->Write( data, size );
	
	if( bLogOutput_Write )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Output_CanWrite
////////////////////////////////////////////////////////////////////////////////
int Output_CanWrite()
{
	if( bLogOutput_CanWrite )
	{
		Console::Append( "Output::CanWrite()" );
	}

	int res = g_InModMaster.outMod->CanWrite();
	
	if( bLogOutput_CanWrite )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Output_IsPlaying
////////////////////////////////////////////////////////////////////////////////
int Output_IsPlaying()
{
	if( bLogOutput_IsPlaying )
	{
		Console::Append( "Output::IsPlaying()" );
	}

	int res = g_InModMaster.outMod->IsPlaying();
	
	if( bLogOutput_IsPlaying )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Output_Pause
////////////////////////////////////////////////////////////////////////////////
int Output_Pause( int new_state )
{
	if( bLogOutput_Pause )
	{
		Console::Append( "Output::Pause( int new_state )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   new_state = %i", new_state );
		Console::Append( szBuffer );
	}

	int res = g_InModMaster.outMod->Pause( new_state );
	
	if( bLogOutput_Pause )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}	



////////////////////////////////////////////////////////////////////////////////
//  Output_SetVolume
////////////////////////////////////////////////////////////////////////////////
void Output_SetVolume( int v )
{
	if( bLogOutput_SetVolume )
	{
		Console::Append( "Output::SetVolume( int v )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   v = %i", v );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_InModMaster.outMod->SetVolume( v );
}



////////////////////////////////////////////////////////////////////////////////
//  Output_SetPan
////////////////////////////////////////////////////////////////////////////////
void Output_SetPan( int p )
{
	if( bLogOutput_SetPan )
	{
		Console::Append( "Output::SetPan( int p )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   p = %i", p );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_InModMaster.outMod->SetPan( p );
}



////////////////////////////////////////////////////////////////////////////////
//  Output_Flush
////////////////////////////////////////////////////////////////////////////////
void Output_Flush( int pos )
{
	if( bLogOutput_Flush )
	{
		Console::Append( "Output::Flush( int pos )" );
		char szBuffer[ 500 ];
		sprintf( szBuffer, "   pos = %i", pos );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	g_InModMaster.outMod->Flush( pos );
}



////////////////////////////////////////////////////////////////////////////////
//  Output_GetOutputTime
////////////////////////////////////////////////////////////////////////////////
int Output_GetOutputTime()
{
	if( bLogOutput_GetOutputTime )
	{
		Console::Append( "Output::GetOutputTime()" );
	}

	int res = g_InModMaster.outMod->GetOutputTime();
	
	if( bLogOutput_GetOutputTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



////////////////////////////////////////////////////////////////////////////////
//  Output_GetWrittenTime
////////////////////////////////////////////////////////////////////////////////
int Output_GetWrittenTime()
{
	if( bLogOutput_GetWrittenTime )
	{
		Console::Append( "Output::GetWrittenTime()" );
	}

	int res = g_InModMaster.outMod->GetWrittenTime();

	if( bLogOutput_GetWrittenTime )
	{
		char szBuffer[ 500 ];
		sprintf( szBuffer, "result = %i", res );
		Console::Append( szBuffer );
		Console::Append( " " );
	}
	
	return res;
}



//////////////////////////////////////////////////////////////////////////////// 
//  RepairTimerProc
//////////////////////////////////////////////////////////////////////////////// 
VOID CALLBACK RepairTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	static bool bStayOut = false;

	if( bStayOut )
	{
		return;
	}
	
	bStayOut = true;
//////////////////////////////////////////////////////////////////////////////// 
	
	// New output plugin?
	static Out_Module * old_out = NULL;
	if( g_InModMaster.outMod != old_out )
	{
		// Copy possibly changed properties
		g_OutModMaster.description   = g_InModMaster.outMod->description;
		g_OutModMaster.id            = g_InModMaster.outMod->id;
		g_OutModMaster.hDllInstance  = g_InModMaster.outMod->hDllInstance;

		Console::Append( ">> Output plugin change detected <<" );
		Console::Append( " " );
		
		old_out = g_InModMaster.outMod;
	}
	
	
	// New props of input plugin?
	if( g_pInModSlave->FileExtensions != g_InModMaster.FileExtensions )
	{
		g_InModMaster.FileExtensions = g_pInModSlave->FileExtensions;
		Console::Append( ">> File extensions change detected <<" );
		Console::Append( " " );
	}

	if( g_pInModSlave->is_seekable != g_InModMaster.is_seekable )
	{
		g_InModMaster.is_seekable = g_pInModSlave->is_seekable;
		Console::Append( ">> Seekable flag change detected <<" );
		Console::Append( " " );
	}
	
	if( g_pInModSlave->UsesOutputPlug != g_InModMaster.UsesOutputPlug )
	{
		g_InModMaster.UsesOutputPlug = g_pInModSlave->UsesOutputPlug;
		Console::Append( ">> Output-Plugin-Usage flag change detected <<" );
		Console::Append( " " );
	}
	
////////////////////////////////////////////////////////////////////////////////
 	bStayOut = false;
}



//////////////////////////////////////////////////////////////////////////////// 
//  MainHandleTimerProc
//////////////////////////////////////////////////////////////////////////////// 
VOID CALLBACK MainHandleTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	bool bStayOut = false;

	if( bStayOut )
	{
		return;
	}
	
	if( g_InModMaster.hMainWindow )
	{
		bStayOut = true;
//////////////////////////////////////////////////////////////////////////////// 

		KillTimer( NULL, hMainHandleTimer );
		hMainHandleTimer = 0;

		g_pInModSlave->hMainWindow = g_InModMaster.hMainWindow;

		Console::Append( ">> Main window handle found <<" );
		Console::Append( " " );

//////////////////////////////////////////////////////////////////////////////// 
		bStayOut = false;
	}
}



///////////////////////////////////////////////////////////////////////////////
//  winampGetInModule2
////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec( dllexport ) In_Module * winampGetInModule2()
{
	g_hMasterInstance = ( HINSTANCE )GetCurrentModule();
	g_InModMaster.hDllInstance = g_hMasterInstance;


	// Get master path
	char szFullpath[ MAX_PATH ] = "";
	GetModuleFileName( g_hMasterInstance, szFullpath, MAX_PATH - 6 - 1 );
	const int iFullLathLen = strlen( szFullpath );

	// Get slave path
	char * walk = szFullpath + iFullLathLen - 1; // Last char
	while( ( walk > szFullpath ) && ( *walk != '\\' ) ) walk--;
	walk++;
	
	// Backup basename e.g. "in_mp3.dll"
	const int iBackupLen = ( szFullpath + iFullLathLen ) - walk;
	char * szBackup = new char[ iBackupLen + 1 ];
	memcpy( szBackup, walk, iBackupLen );
	szBackup[ iBackupLen ] = '\0';
	
	// Make plugin.ini path for config
	memcpy( walk, "plugin.ini\0", 11 );

	// Make section name for config
	char * szSection = new char[ 10 + iBackupLen + 1 ];
	sprintf( szSection, "Inbridge, %s", szBackup );

	// Read config
	g_pCONFIG = new DEVIL_CONFIG( szSection, szFullpath );
	ReadConfig();

	// Make slave plugin path
	memcpy( walk, "slave_", 6 );
	walk += 6;
	memcpy( walk, szBackup, iBackupLen );
	walk[ iBackupLen ] = '\0';
	char * szBasename = walk;

	// Not needed anymore
    delete [] szBackup;
    delete [] szSection;


	// Load slave dll
	g_hSlaveInstance = LoadLibrary( szFullpath );
	if( !g_hSlaveInstance )
	{
		_strlwr( szBasename );
		char szBuffer[ 1000 ];
		wsprintf(
			szBuffer,
			"Slave plugin could not be loaded:\n"
			"    slave_%s  \n"
			"\n"
			"NOTE:\tRenaming the master file also affects  \n"
			"\tthe expected filename of the slave.",
			szBasename
		);
		MessageBox( NULL, szBuffer, "Slave plugin error", MB_ICONINFORMATION );
		return NULL;
	}


	// Find export
	WINAMPGETINMODULE2 winampGetOutModule2 =
		( WINAMPGETINMODULE2 )GetProcAddress( g_hSlaveInstance, "winampGetInModule2" );
	if( !winampGetOutModule2 )
	{
		FreeLibrary( g_hSlaveInstance );
		return NULL;
	}


	// Get module
	g_pInModSlave = winampGetOutModule2();
	if( !g_pInModSlave )
	{
		FreeLibrary( g_hSlaveInstance );
		return NULL;
	}


	// Version mismatch?
	if( g_pInModSlave->version != IN_VER )
	{
		FreeLibrary( g_hSlaveInstance );
		return NULL;
	}
	

	// Modify slave
	g_pInModSlave->hDllInstance   = g_hSlaveInstance;
	g_pInModSlave->outMod         = &g_OutModMaster;
	
	// Modfiy master
	g_InModMaster.description     = g_pInModSlave->description;
	g_InModMaster.FileExtensions  = g_pInModSlave->FileExtensions;
	g_InModMaster.is_seekable     = g_pInModSlave->is_seekable;
	g_InModMaster.UsesOutputPlug  = g_pInModSlave->UsesOutputPlug;
	

	// Initialize console
	Console::Create( szBasename );
	Console::Append( "Inbridge Winamp Plugin 2.2" );
	Console::Append( "http://www.hartwork.org" );
	Console::Append( " " );
	Console::Append( "Right-click for settings" );
	Console::Append( "=================================================" );
	Console::Append( " " );


	// Start main window detection
	hMainHandleTimer = SetTimer( NULL, 0, 333, MainHandleTimerProc );

	// Start repair "thread"
	hRepairTimer = SetTimer( NULL, 0, 500, RepairTimerProc );


	return &g_InModMaster;
}
