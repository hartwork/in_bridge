

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


#ifndef GLOBAL_H
#define GLOBAL_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


#define CONSOLE_DEFAULT_WIDTH   460
#define CONSOLE_DEFAULT_HEIGHT  310
#define CONSOLE_MIN_WIDTH       300
#define CONSOLE_MIN_HEIGHT      200


extern HINSTANCE g_hMasterInstance;

extern bool bLogInput_Config;
extern bool bLogInput_About;
extern bool bLogInput_Init;
extern bool bLogInput_Quit;
extern bool bLogInput_GetFileInfo;
extern bool bLogInput_InfoBox;
extern bool bLogInput_IsOurFile;
extern bool bLogInput_Play;
extern bool bLogInput_Pause;
extern bool bLogInput_UnPause;
extern bool bLogInput_IsPaused;
extern bool bLogInput_Stop;
extern bool bLogInput_GetLength;
extern bool bLogInput_GetOutputTime;
extern bool bLogInput_SetOutputTime;
extern bool bLogInput_SetVolume;
extern bool bLogInput_SetPan;
extern bool bLogInput_EQSet;

extern bool bLogCore_SAVSAInit;
extern bool bLogCore_SAVSADeInit;
extern bool bLogCore_SAAddPCMData;
extern bool bLogCore_SAGetMode;
extern bool bLogCore_SAAdd;
extern bool bLogCore_VSAAddPCMData;
extern bool bLogCore_VSAGetMode;
extern bool bLogCore_VSAAdd;
extern bool bLogCore_VSASetInfo;
extern bool bLogCore_dsp_isactive;
extern bool bLogCore_dsp_dosamples;
extern bool bLogCore_SetInfo;

// extern bool bLogOutput_Config;
// extern bool bLogOutput_About;
// extern bool bLogOutput_Init;
// extern bool bLogOutput_Quit;
extern bool bLogOutput_Open;
extern bool bLogOutput_Close;
extern bool bLogOutput_Write;
extern bool bLogOutput_CanWrite;
extern bool bLogOutput_IsPlaying;
extern bool bLogOutput_Pause;
extern bool bLogOutput_SetVolume;
extern bool bLogOutput_SetPan;
extern bool bLogOutput_Flush;
extern bool bLogOutput_GetOutputTime;
extern bool bLogOutput_GetWrittenTime;

extern int iShowCmd;
extern int xMaxPosition;
extern int yMaxPosition;
extern int xNormalPosition;
extern int yNormalPosition;
extern int cxNormalPosition;
extern int cyNormalPosition;



#endif // GLOBAL_H
