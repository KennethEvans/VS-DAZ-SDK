/**********************************************************************
Copyright (C) 2002-2012 DAZ 3D, Inc. All Rights Reserved.

This file is part of the DAZ Studio SDK.

This file may be used only in accordance with the DAZ Studio SDK 
license provided with the DAZ Studio SDK.

The contents of this file may not be disclosed to third parties, 
copied or duplicated in any form, in whole or in part, without the 
prior written permission of DAZ 3D, Inc, except as explicitly
allowed in the DAZ Studio SDK license.

See http://www.daz3d.com to contact DAZ 3D, Inc or for more 
information about the DAZ Studio SDK.
**********************************************************************/

/*****************************
Include files
*****************************/
#include "dzplugin.h"

#include "version.h"
#include "dzinfopane.h"

/*****************************
Plugin Definition
*****************************/

DZ_PLUGIN_DEFINITION( "Scene Info Pane Ex" );
DZ_PLUGIN_AUTHOR( "Kenneth Evans, kenevans.net" );
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

DZ_PLUGIN_DESCRIPTION(
					  "This plug-in provides a simple HTML-based display of "
					  "extended information about the scene contents and the "
					  "current selection. The pane updates itself whenever "
					  "the scene or primary selection is changed."
					  );

DZ_PLUGIN_CLASS_GUID( DzSceneInfoPaneEx, 1FE6448D-CFE1-483e-8389-127E9017180B );
DZ_PLUGIN_CLASS_GUID( DzSceneInfoPaneExAction, D2303DBE-9FD8-4507-AB4A-4C732E2B3886 );
