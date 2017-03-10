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
#include "dzapp.h"

#include "version.h"
#include "afirstpluginaction.h"

/*****************************
Plugin Definition
*****************************/

/**
This macro creates the plugin definition, and the functions that are needed
for DAZ Studio to load our plugin.
**/
DZ_PLUGIN_DEFINITION( "A First Plugin" );

/**
This macro sets the author string for the plugin
**/
DZ_PLUGIN_AUTHOR( "Kenneth Evans, kenevans.net" );

/**
This macro sets the version number for the plugin
**/
DZ_PLUGIN_VERSION( PLUGIN_MAJOR, PLUGIN_MINOR, PLUGIN_REV, PLUGIN_BUILD );

/**
This macro sets the description string for the plugin. This is a good place
to provide specific information about the plugin, including an HTML link to
any external documentation. Links are shown in the system default browser.
**/
DZ_PLUGIN_DESCRIPTION( QString(
					  "<a href=\"%1/aFirstPlugin/index.htm\">Documentation</a><br><br>"
					  "This plugin is a simple example provided with the DAZ Studio "
					  "SDK. It provides developers an example of the basics with "
					  "regard to project layout, subclassing DzAction, standard "
					  "dialog creation and button layout."
					  ).arg( dzApp->getDocumentationPath() ) );

/**
This macro adds our class to the objects exported by the plugin, and specifies the
GUID (Globally Unique Identifier) that makes this class unique from any other class
that is available from DAZ Studio or one of it's plug-ins. DO NOT USE THE GUID BELOW
IN YOUR PLUG-IN. Make sure that you generate a new GUID for every class that you export
from your plug-in. See the 'ClassIDs' page in the DAZ Studio SDK documentation for more 
information.
**/
DZ_PLUGIN_CLASS_GUID( DzAFirstPluginAction, 02F86BFC-3193-4e43-8101-6DA342EF97D8 );
