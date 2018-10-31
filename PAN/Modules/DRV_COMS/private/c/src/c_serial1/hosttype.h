#ifndef _hosttype_H
#define _hosttype_H
/*
	NI CONFIDENTIAL
	© Copyright 1990-2000 by National Instruments Corp.
	All rights reserved.

	Owners: brian.powell, greg.richardson

	hosttype.h - host specific definitions, etc.
*/

#ifdef DefineHeaderRCSId
static char rcsid_hosttype[] = "$Id: //lvdist/releases/6.0/plat/win/cintools/hosttype.h#1 $";
#endif

#if Mac
	#include <Memory.h>
	#include <Files.h>
	#include <TextEdit.h>
	#include <Windows.h>
	#include <Dialogs.h>
	#include <OSUtils.h>
	#include <Resources.h>
	#include <Events.h>
	#include <Traps.h>
	#include <QDOffscreen.h>
	#include <Menus.h>
	#include <Packages.h>
	#include <Printing.h>
	#include <Palettes.h>
	#include <SegLoad.h>
	#include <LowMem.h>
	#include <Gestalt.h>
	#include <Errors.h>

	#if OpSystem != kMacCarbon
		#define QDG(qdField)	(qd.qdField)
	#endif

#endif	/* Mac */

#if OpenStep
#include <AppKit/NSWindow.h>
#endif /* OpenStep */

#if Unix
#if AlphaPtr32
	#pragma pointer_size 64
#endif
#include <stdio.h>
#include <string.h>
#if XWindows
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif /* XWindows */
#if AlphaPtr32
	#pragma pointer_size 32
#endif
#endif /* Unix */

#if WindowSystem == kBeWS
	#include <Window.h>
	#include <View.h>
	#include <Bitmap.h>
#endif /* kBeWS */

#if MSWin
#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

#define _WIN32_WINNT 0x0400
#include <windows.h>
#pragma warning (disable : 4001) /* nonstandard extension 'single line comment' was used */

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1L
#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0L

#endif /* MSWin */

#endif /* _hosttype_H */
