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
#include "dzinfopane.h"

#include <QtCore/QObject>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>

#include "dzapp.h"
#include "dzbone.h"
#include "dzcamera.h"
#include "dzlight.h"
#include "dzfacetmesh.h"
#include "dzhelpmgr.h"
#include "dzobject.h"
#include "dzscene.h"
#include "dzshape.h"
#include "dzskeleton.h"
#include "dzstyle.h"
#include "dzproperty.h"
#include "dztarray.h"
#include "dzactionmenu.h"

static const int c_minWidth = 200;
static const int c_minHeight = 150;

// Function prototypes
bool compareMaterials(const QObject *obj1, const QObject *obj2);

/**
**/
DzSceneInfoPaneEx::DzSceneInfoPaneEx() :
DzPane( "Scene Info Ex" ),
m_refreshBlocked( false ), 
m_totalVerts( 0 ),
m_totalTris( 0 ),
m_totalQuads( 0 ),
m_showScene (false),
m_showNodes (true),
m_showSelected (false),
m_showSelectedObject (false),
m_showSelectedProperties (false),
m_showMaterials( false )
{
	// Declarations
	int margin = style()->pixelMetric( DZ_PM_GeneralMargin );

	// Define the layout for the pane
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setMargin( margin );
	mainLayout->setSpacing( margin );

	// Use a text browser for the output window - this supports basic html/rtf formatting
	m_output = new QTextBrowser();
	m_output->setObjectName( "SceneInfoExTxtBrwsr" );
	m_output->setMinimumSize( c_minWidth, c_minHeight );

	// Implement the context menu
	m_output->setContextMenuPolicy(Qt::CustomContextMenu);
	connect( m_output, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)) );

	mainLayout->addWidget( m_output );

	// Set the layout for the pane
	setLayout( mainLayout );

	// Do an initial update of the scene stats
	refresh();

	// Listen for the node list in the scene to change so that we can update our information
	connect( dzScene, SIGNAL(nodeListChanged()), this, SLOT(refresh()) );

	// Listen for the primary selection to change so that we can update our information
	connect( dzScene, SIGNAL(primarySelectionChanged(DzNode*)), this, SLOT(refreshInfo(DzNode*)) );

	// Listen for file load operations so that we can block our refresh function from executing.
	// During a scene load, the node list will change many times - we only want to refresh
	// once at the end of the load, so that we do not slow down the file load.
	connect( dzScene, SIGNAL(sceneLoadStarting()), this, SLOT(blockRefresh()) );
	connect( dzScene, SIGNAL(sceneClearStarting()), this, SLOT(blockRefresh()) );
	connect( dzScene, SIGNAL(sceneLoaded()), this, SLOT(unblockRefresh()) );
	connect( dzScene, SIGNAL(sceneCleared()), this, SLOT(unblockRefresh()) );

	setMinimumSize( c_minWidth, c_minHeight );
}

/**
**/
DzSceneInfoPaneEx::~DzSceneInfoPaneEx() {
}

/**
**/
void DzSceneInfoPaneEx::showContextMenu(const QPoint &pos) {
	QMenu menu;
	QString sceneText = m_showScene ? 
		"Hide Scene" : "Show Scene";
	QString nodesText = m_showNodes ? 
		"Hide Nodes" : "Show Nodes";
	QString selectedText = m_showSelected ? 
		"Hide Selected" : "Show Selected";
	QString objectText = m_showSelectedObject ? 
		"Hide Selected Object" : "Show Selected Object";
	QString propertiesText = m_showSelectedProperties ? 
		"Hide Selected Properties" : "Show Selected Properties";
	QString materialsText = m_showMaterials ? 
		"Hide Materials" : "Show Materials";

	QAction *sceneAction = menu.addAction(sceneText);
	QAction *nodesAction = menu.addAction(nodesText);
	QAction *selectedAction = menu.addAction(selectedText);
	QAction *objectAction = menu.addAction(objectText);
	QAction *propertiesAction = menu.addAction(propertiesText);
	QAction *materialsAction = menu.addAction(materialsText);
	menu.addSeparator();
	QAction *refreshAction = menu.addAction("Refresh");

	QPoint globalPos = m_output->viewport()->mapToGlobal(pos);
	QAction *selectedItem = menu.exec(globalPos);
	if(!selectedItem) {
		return;
	}
	if(selectedItem == sceneAction) {
		m_showScene = !m_showScene;
		refresh();
	} else if(selectedItem == nodesAction) {
		m_showNodes = !m_showNodes;
		refresh();
	} else if(selectedItem == selectedAction) {
		m_showSelected = !m_showSelected;
		refresh();
	} else if(selectedItem == objectAction) {
		m_showSelectedObject = !m_showSelectedObject;
		refresh();
	} else if(selectedItem == propertiesAction) {
		m_showSelectedProperties = !m_showSelectedProperties;
		refresh();
	} else if(selectedItem == materialsAction) {
		m_showMaterials = !m_showMaterials;
		refresh();
	} else if(selectedItem == refreshAction) {
		refresh();
	}
}

/**
**/
void DzSceneInfoPaneEx::blockRefresh() {
	// A scene file is being loaded - block the refresh functions
	m_refreshBlocked = true;
}

/**
**/
void DzSceneInfoPaneEx::unblockRefresh() {
	// A scene file just finished loading - unblock the refresh functions, 
	// and force an update of our data.
	m_refreshBlocked = false;
	refresh();
}

/**
**/
void DzSceneInfoPaneEx::getNodeInfo( const DzNode *node, int &numVerts, int &numTris, int &numQuads ) {
	// Collect information about the node's geometry
	DzObject	*obj = node->getObject();
	DzShape		*shape = obj ? obj->getCurrentShape() : NULL;
	DzGeometry	*geom = shape ? shape->getGeometry() : NULL;
	numVerts = 0;
	numTris = 0;
	numQuads = 0;

	if( shape == NULL || geom == NULL ) {
		return;	// No geometry for the node
	}

	numVerts = geom->getNumVertices();
	DzFacetMesh	*mesh = qobject_cast<DzFacetMesh*>( geom );
	if( mesh ) {
		int		i, nFacets = mesh->getNumFacets();
		DzFacet	*facets = mesh->getFacetsPtr();

		for( i = 0; i < nFacets; i++ ) {
			if( facets[i].isQuad() )
				numQuads++;
			else
				numTris++;
		}
	}
}

/**
**/
void DzSceneInfoPaneEx::refresh() {
	if( m_refreshBlocked ) {
		// Ignore signals during scene load
		return;
	}

	int	numVerts, numTris, numQuads;
	DzNodeListIterator nodeIter( dzScene->nodeListIterator() );

	// Clear out any previous data
	m_output->clear();
	m_totalVerts = 0;
	m_totalTris = 0;
	m_totalQuads = 0;

	// Iterate over all the nodes in the scene and collect data for them
	while( nodeIter.hasNext() ) {
		getNodeInfo( nodeIter.next(), numVerts, numTris, numQuads );
		m_totalVerts += numVerts;
		m_totalTris += numTris;
		m_totalQuads += numQuads;
	}

	// Ignore signals during scene load
	if( m_refreshBlocked ) {
		return;
	}

	// Refresh the output
	refreshInfo( dzScene->getPrimarySelection() );
}

/**
**/
void DzSceneInfoPaneEx::refreshInfo( DzNode *node ) {
	if( m_refreshBlocked ) {
		// Ignore signals during scene load
		return;
	}

	// Clear out any previous data
	m_output->clear();

	// Write data for the scene
	if(m_showScene) {
		writeSceneInfo();
	}

	// Write data for the scene nodes
	if(m_showNodes) {
		writeNodes();
	}

	// Write data for the primary selection
	if(m_showSelected) {
		writeSelectedNode( dzScene->getPrimarySelection() );
	}

	// Write data for the object associated with the node
	if(m_showSelectedObject) {
		writeSelectedObjectInfo(node);
	}

	// Write data for the properties associated with the node
	if(m_showSelectedProperties) {
		writeSelectedPropertyInfo(node);
	}

	// Write the materials
	if(m_showMaterials) {
		writeMaterialInfo();
	}
}

/**
**/
void DzSceneInfoPaneEx::writeSceneInfo() {
	QString	html;

#if 0
	// DEBUG
	// Context menu
	DzActionMenu *menu = getOptionsMenu();
	menu->getNumItems();
	if(menu) {
		html += QString( "<b>Context Menu : Items=%1</b><br>" )
			.arg( menu->getNumItems() );
	}
#endif

	// DEBUG
	// SDK Version
	html += QString( "<b>DAZ SDK Version : %1.%2.%3.%4</b><br>" )
		.arg( DZ_VERSION_MAJOR )
		.arg( DZ_VERSION_MINOR )
		.arg( DZ_VERSION_REV )
		.arg( DZ_VERSION_BUILD );

	// DEBUG
	// QT Version
	const char *qtVersion = qVersion();
	html += QString( "<b>QT Version : %1</b><br><br>" ).arg( qtVersion );

	// Generate the scene info table:
	html += "<b>Scene Items : </b><br><table>";
	html += QString( "<tr><td>Nodes : </td><td>%1</td></tr>" )
		.arg( dzScene->getNumNodes() );
	html += QString( "<tr><td>Lights : </td><td>%1</td></tr>" )
		.arg( dzScene->getNumLights() );
	html += QString( "<tr><td>Cameras : </td><td>%1</td></tr>" )
		.arg( dzScene->getNumCameras() );
	html += QString( "<tr><td>World-Space Modifiers : </td><td>%1</td></tr>" )
		.arg( dzScene->getNumWSModifiers() );
	html += "</table><br>";

	// Generate the scene geometry table:
	html += "<br>";
	html += "<b>Scene Geometry : </b><br><table>";
	html += QString( "<tr><td>Total Vertices : </td><td>%1</td></tr>" )
		.arg( m_totalVerts );
	html += QString( "<tr><td>Total Triangles : </td><td>%1</td></tr>" )
		.arg( m_totalTris );
	html += QString( "<tr><td>Total Quads : </td><td>%1</td></tr>" )
		.arg( m_totalQuads );
	html += QString( "<tr><td>Total Faces : </td><td>%1</td></tr>" )
		.arg( m_totalTris + m_totalQuads );
	html += "</table><br>";

	m_output->append( html );
}

/**
**/
void DzSceneInfoPaneEx::writeNodes() {
	QString	html;

	// Generate the nodes table
	int nNodes = dzScene->getNumNodes();
	html += QString( "<b>Nodes : %1</b><br><table>" )
		.arg( nNodes );

	// Iterate over all the nodes in the scene
	DzNodeListIterator nodeIter( dzScene->nodeListIterator() );
	DzNode *node = NULL;
	DzSceneAssetFileInfo fileInfo = NULL;
	while( nodeIter.hasNext() ) {
		node = nodeIter.next();
		fileInfo = node->getAssetFileInfo();
		html += QString( "<tr><td>%1 : </td><td>%2</td></tr>" )
			.arg( node->getLabel() )
			.arg( fileInfo.getUri().getFilePath() );
	}
	html += "</table><br>";

	// Iterate over all the lights in the scene
	int nLights = dzScene->getNumLights();
	html += "<br>";
	html += QString( "<b>Lights : %1</b><br><table>" )
		.arg( nLights );
	DzLightListIterator lightIter( dzScene->lightListIterator() );
	DzLight *light = NULL;
	while( lightIter.hasNext() ) {
		light = lightIter.next();
		html += QString( "<tr><td>%1 : </td><td>%2 %3 %4</td><td>%5 %6</td></tr>" )
			.arg( light->getLabel() )
			.arg(light->getDiffuseColor().red())
			.arg(light->getDiffuseColor().green())
			.arg(light->getDiffuseColor().blue())
			.arg( light->isAreaLight()?"Area":"" )
			.arg( light->isDirectional()?"Directional":"" );
	}
	html += "</table><br>";

	// Iterate over all the cameras in the scene
	int nCameras = dzScene->getNumCameras();
	html += "<br>";
	html += QString( "<b>Cameras : %1</b><br><table>" )
		.arg( nCameras );
	DzCameraListIterator cameraIter( dzScene->cameraListIterator() );
	DzCamera *camera = NULL;
	while( cameraIter.hasNext() ) {
		camera = cameraIter.next();
		html += QString( "<tr><td>%1 : </td><td>Focal Distance %2</td>"
			"<td>Focal Length %3 mm</td></tr>" )
			.arg( camera->getLabel() )
			.arg( camera->getFocalDistance() )
			.arg( camera->getFocalLength() );
	}
	html += "</table><br>";

	m_output->append( html );
}

/**
**/
void DzSceneInfoPaneEx::writeSelectedNode( DzNode *node ) {
	if( m_refreshBlocked ) {
		// Ignore signals during scene load
		return;
	}

	int		tNumVerts = 0, tNumTris = 0, tNumQuads = 0;
	QString	html;

	if( node ) {
		// Get the stats for the primary selection
		getNodeInfo( node, tNumVerts, tNumTris, tNumQuads );

		if( node->inherits( "DzSkeleton" ) ) {
			// If the primary selection is a figure, add the stats for all the bones also
			DzBoneList bones;
			int	i, n, numVerts, numTris, numQuads;

			((DzSkeleton*)node)->getAllBones( bones );
			for( i = 0, n = bones.count(); i < n; i++ ) {
				getNodeInfo( bones[i], numVerts, numTris, numQuads );
				tNumVerts += numVerts;
				tNumTris += numTris;
				tNumQuads += numQuads;
			}
		}

		// Write the html output for the primary selection stats
		html += "<b>Primary Selection : </b><br><table>";
		html += QString( "<tr><td>Name : </td><td>%1</td></tr>" ).arg( node->objectName() );
		html += QString( "<tr><td>Label : </td><td>%1</td></tr>" ).arg( node->getLabel() );
		html += QString( "<tr><td>Class : </td><td>%1</td></tr>" ).arg( node->className() );
		html += QString( "<tr><td>Asset ID : </td><td>%1</td></tr>" ).arg( node->getAssetId() );
		html += QString( "<tr><td>Asset Type : </td><td>%1</td></tr>" ).arg( node->getAssetType() );
		html += QString( "<tr><td>Data Items : </td><td>%1</td></tr>" ).arg( node->getNumDataItems() );
		html += QString( "<tr><td>Element Children : </td><td>%1</td></tr>" ).arg( node->getNumElementChildren() );
		html += QString( "<tr><td>Node Children : </td><td>%1</td></tr>" ).arg( node->getNumNodeChildren() );
		html += QString( "<tr><td>Properties : </td><td>%1</td></tr>" ).arg( node->getNumProperties() );
		html += QString( "<tr><td>Private Properties : </td><td>%1</td></tr>" ).arg( node->getNumPrivateProperties() );
		html += QString( "<tr><td>Source : </td><td>%1</td></tr>" )
			.arg( node->getSource().getFilePath() );
		html += QString( "<tr><td>File Path : </td><td>%1</td></tr>" )
			.arg( node->getAssetFileInfo().getUri().getFilePath() );
		html += QString( "<tr><td>Vertices : </td><td>%1</td></tr>" ).arg( tNumVerts );
		html += QString( "<tr><td>Triangles : </td><td>%1</td></tr>" ).arg( tNumTris );
		html += QString( "<tr><td>Quads : </td><td>%1</td></tr>" ).arg( tNumQuads );
		html += QString( "<tr><td>Total Faces : </td><td>%1</td></tr>" ).arg( tNumTris + tNumQuads );
		html += "</table><br>";
		m_output->append( html );
	} else {
		// No primary selection
		html += "<br>";
		html += "<b>Primary Selection : </b>None<br>";
		m_output->append( html );
	}
}

/**
**/
void DzSceneInfoPaneEx::writeSelectedObjectInfo( DzNode *node ) {
	if( m_refreshBlocked ) {
		// Ignore signals during scene load
		return;
	}
	if(!node) {
		return;
	}

	QString	html;
	DzObject *obj = node ->getObject();
	html += QString( "<b>Primary Selection - Object : %1</b><br>" )
		.arg( obj ? "" : "No Object" );
	if(obj == NULL) {
		m_output->append( html );
		return;
	}

	int nShapes = obj->getNumShapes();
	html += "<table>";
	html += QString( "<tr><td>Total Shapes : </td><td>%1</td></tr>" )
		.arg( nShapes );
	DzShape *shape = NULL;
	DzMaterial *material = NULL;
	DzTexture *texture = NULL;
	QString fileName = QString();
	int nMaterials;
	for(int i = 0; i < nShapes; i++) {
		shape = obj->getShape(i);
		nMaterials = shape->getNumMaterials();
#if 1
		QObjectList materialsList0;
		materialsList0 = shape->getAllMaterials();
		nMaterials = materialsList0.count();

		// Make a new QObjectList and sort it
		QObjectList materialsList;
		for(int j = 0; j < nMaterials; j++) {
			material = (DzMaterial *)materialsList0[j];
			materialsList.append(material);
		}
		qSort(materialsList.begin(), materialsList.end(), compareMaterials);

		html += QString( "<tr><td>Shape %1 : </td><td>Materials : %2</td></tr>" )
			.arg( i )
			.arg(nMaterials);
		for(int j = 0; j < nMaterials; j++) {
			material = (DzMaterial *)materialsList[j];
			texture = material->getColorMap();
			fileName = QString();
			if(texture) {
				fileName = texture->getFilename();
			}
			if(fileName.isEmpty()) {
				// Show diffuse color
				html += QString( "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;%1 : </td><td>Diffuse : %2 %3 %4</td></tr>" )
					.arg( material->getName() )
					.arg(material->getDiffuseColor().red())
					.arg(material->getDiffuseColor().green())
					.arg(material->getDiffuseColor().blue());
			} else {
				// Show filename
				html += QString( "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;%1 : </td><td>%2</td></tr>" )
					.arg( material->getName() )
					.arg(fileName);
			}
		}
#else
		// The preferred way, but isn't working
		DzMaterialPtrList matPtrs = NULL;
		shape->getAllMaterials(matPtrs);
		html += QString( "<tr><td>Shape %1 : </td><td>Count %2</td></tr>" )
			.arg( i )
			.arg(matPtrs.count());
		for(int j = 0; j < matPtrs.count(); j++) {
			material = (DzMaterial *)matPtrs[j];
			html += QString( "<tr><td>%1 : </td><td></td></tr>" )
				.arg( material->getName() );
		}
#endif
	}
	html += "</table><br>";
	m_output->append( html );
}

/**
**/
void DzSceneInfoPaneEx::writeMaterialInfo() {
	if( m_refreshBlocked ) {
		// Ignore signals during scene load
		return;
	}

	int nMaterials = DzMaterial::getNumMaterials();

	QString	html;
	html += QString( "<b>All Materials : %1</b><br><table>" ).arg( nMaterials );

	// Make a QObjectList of the materials and sort it
	QObjectList materialsList;
	DzMaterial *material = NULL;
	for(int j = 0; j < nMaterials; j++) {
		material = DzMaterial::getMaterial(j);
		materialsList.append(material);
	}
	qSort(materialsList.begin(), materialsList.end(), compareMaterials);

	DzTexture *texture = NULL;
	QColor color;
	QString fileName = QString();
	QString name;
	for(int j = 0; j < nMaterials; j++) {
		material = (DzMaterial *)materialsList[j];
		if(material == NULL) {
			html += QString( "<tr><td>%1 : </td><td>%2</td></tr>" )
				.arg(j)
				.arg( "NULL" );
			continue;
		}

		name = material->getName();
		if(name.isEmpty()) {
			name = "<Blank>";
		}
		if(name.isNull()) {
			name = "<Null>";
		}
		html += QString( "<tr><td>%1 [%2] : </td><td>%3</td></tr>" )
			.arg(j)
			.arg(material->getIndex())
			.arg( name );

		// Color map
		texture = material->getColorMap();
		fileName = QString();
		if(texture) {
			fileName = texture->getFilename();
		}
		if(!fileName.isEmpty()) {
			html += QString( "<tr><td></td><td>Color File : </td><td>%2 : </td></tr>" )
				.arg(fileName);
		}

		// Opacity map
		texture = material->getOpacityMap();
		fileName = QString();
		if(texture) {
			fileName = texture->getFilename();
		}
		if(!fileName.isEmpty()) {
			html += QString( "<tr><td></td><td>Opacity File : </td><td>%2 : </td></tr>" )
				.arg(fileName);
		}

		// Baked map
		texture = material->getBakedMap();
		fileName = QString();
		if(texture) {
			fileName = texture->getFilename();
		}
		if(!fileName.isEmpty()) {
			html += QString( "<tr><td></td><td>Baked File : </td><td>%2 : </td></tr>" )
				.arg(fileName);
		}

		// Diffuse color
		color = material->getDiffuseColor();
		html += QString( "<tr><td></td><td>Diffuse Color : </td><td>%1 %2 %3</td></tr>" )
			.arg(color.red())
			.arg(color.green())
			.arg(color.blue());
	}

	html += "</table><br>";
	m_output->append( html );
}

bool compareMaterials(const QObject *obj1, const QObject *obj2) {
	DzMaterial *material1 =  (DzMaterial *)obj1;
	DzMaterial *material2 =  (DzMaterial *)obj2;
	if(!material1) return false;
	if(!material2) return true;
	return material1->getName() < material2->getName();
}

/**
**/
void DzSceneInfoPaneEx::writeSelectedPropertyInfo( DzNode *node )
{
	if( m_refreshBlocked ) {
		// Ignore signals during scene load
		return;
	}
	if(!node) {
		return;
	}

	QString	html;

	// Properties
	int nProperties = node->getNumProperties();
	html += QString( "<b>Primary Selection - Properties : %1</b><br><table>" ).arg( nProperties );

	DzPropertyListIterator propIter( node->propertyListIterator() );
	DzProperty *prop = NULL;
	while( propIter.hasNext() ) {
		prop = propIter.next();
		// Could consider getSource()
		html += QString( "<tr><td>%1 : </td><td>%2</td></tr>" )
			.arg( prop->getLabel() )
			.arg( prop->getPath() );
	}
	html += "</table><br>";

	// Private properties
	nProperties = node->getNumPrivateProperties();
	html += "<br>";
	html += QString( "<b>Primary Selection - Private Properties : %1</b><br><table>" ).arg( nProperties );

	DzPropertyListIterator privPropIter( node->privatePropertyListIterator() );
	while( privPropIter.hasNext() ) {
		prop = privPropIter.next();
		html += QString( "<tr><td>%1 : </td><td>%2</td></tr>" )
			.arg( prop->getLabel() )
			.arg( prop->getPath() );
	}
	html += "</table><br>";

	m_output->append( html );

}
