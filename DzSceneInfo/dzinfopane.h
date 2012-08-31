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

#ifndef DAZ_INFO_PANE_H
#define DAZ_INFO_PANE_H

/*****************************
   Include files
*****************************/
#include "dzpane.h"
#include "dzaction.h"

/****************************
   Forward declarations
****************************/
class QTextBrowser;
class DzNode;

/*****************************
   Class definitions
*****************************/
/**
	This class is the menu action that will toggle the pane on/off in the interface
**/
class DzSceneInfoPaneExAction : public DzPaneAction {
	Q_OBJECT
public:
	DzSceneInfoPaneExAction() : DzPaneAction( "DzSceneInfoPaneEx" ) { }
};

/**
	This class is the interface pane
**/
class DzSceneInfoPaneEx : public DzPane {
	Q_OBJECT

public:
	//
	// CREATORS
	//

    DzSceneInfoPaneEx();
    ~DzSceneInfoPaneEx();

public slots:

	/////////////////////////////
	// from DzPane

	virtual void	refresh();

private slots:

	// slots for refreshing/redrawing our data
	void			refreshInfo( DzNode *node );

	// slots for blocking/unblocking our refresh functions during file load operations
	void			blockRefresh();
	void			unblockRefresh();

	void			showContextMenu(const QPoint &);

private:
	//
	// DATA MEMBERS
	//

	bool			m_refreshBlocked;
	QTextBrowser	*m_output;
	int				m_totalVerts, m_totalTris, m_totalQuads;

	bool			m_showScene;
	bool			m_showNodes;
	bool			m_showSelected;
	bool			m_showSelectedObject;
	bool			m_showSelectedProperties;
	bool			m_showMaterials;

	void			writeSceneInfo();
	void			writeNodes();
	void			writeSelectedNode( DzNode *node );
	void			writeSelectedObjectInfo( DzNode *node );
	void			writeSelectedPropertyInfo( DzNode *node );
	void			writeMaterialInfo();
	void			getNodeInfo( const DzNode *node, int &numVerts, int &numTris, int &numQuads );
};

#endif // DAZ_INFO_PANE_H


