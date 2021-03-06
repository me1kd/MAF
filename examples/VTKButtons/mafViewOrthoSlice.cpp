/*
 *  mafViewOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewOrthoSlice.h"
#include "mafOrthoSlice.h"
#include <mafPipeVisual.h>
#include <mafToolHandler.h>
#include <mafBounds.h>
#include <mafVisitorBounds.h>
#include <mafPoint.h>
#include <mafObjectRegistry.h>

using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafCore;
using namespace mafEventBus;

mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafViewCompound(code_location) {
    setConfigurationFile("OrthoSlice.xml");
    m_SlicePosition = new mafPoint();
    m_OrthoPlaneTool.clear();
    m_GUI = new mafOrthoSlice();
    this->setWidget(m_GUI);
    connect(m_GUI, SIGNAL(positionUpdated(double *)), this, SLOT(guiUpdatePosition(double *)));
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
    mafDEL(m_SlicePosition);

    Q_FOREACH(mafToolVTKOrthoPlane *op, m_OrthoPlaneTool) {
        mafDEL(op);
    }
    m_OrthoPlaneTool.clear();
}

void mafViewOrthoSlice::addPlaneToolsToHandler() {
    Q_FOREACH(mafView *v, *viewList()) {
        mafToolVTKOrthoPlane * orthoPlaneTool = mafNEW(mafToolVTKOrthoPlane);
        orthoPlaneTool->setFollowSelectedObject(false);
        orthoPlaneTool->setFollowSelectedObjectVisibility(false);
        orthoPlaneTool->setOrigin(m_SlicePosition);
        mafToolHandler *handler = v->toolHandler();
        handler->addTool(orthoPlaneTool);
        m_OrthoPlaneTool.push_back(orthoPlaneTool);
        connect(orthoPlaneTool, SIGNAL(modifiedObject()), this, SLOT(widgetUpdatePosition()));
    }
}

void mafViewOrthoSlice::widgetUpdatePosition() {
    mafToolVTKOrthoPlane *op = qobject_cast<mafToolVTKOrthoPlane *>(QObject::sender());
    if(op) {
        mafPoint *o = op->origin();
        double p[3];
        o->pos(p);
        m_SlicePosition->setPosition(p);
        Q_FOREACH(mafToolVTKOrthoPlane *plane, m_OrthoPlaneTool) {
            if (plane == op) {
                continue;
            }
            plane->setOrigin(m_SlicePosition);
        }
        double pos[3];
        m_SlicePosition->pos(pos);
        m_GUI->setPosition(pos);
        updateSlice();
    }
}

void mafViewOrthoSlice::guiUpdatePosition(double *pos) {
    m_SlicePosition->setPosition(pos);
    
    Q_FOREACH(mafToolVTKOrthoPlane *op, m_OrthoPlaneTool) {
        op->setOrigin(m_SlicePosition);
    }
    updateSlice();
}

void mafViewOrthoSlice::updateSlice() {
    // Trigger the pipe update
    setModified();
    updateView();
}

void mafViewOrthoSlice::showSceneNode(mafSceneNode *node, bool show) {
    Superclass::showSceneNode(node, show);
    mafVME *vme = node->vme();    
    if (show) {
        m_VisibleVMEsList.push_back(vme);
    } else {
        m_VisibleVMEsList.removeOne(vme);
    }
    
    /// update total visible bounds
    mafVisitorBounds *v = new mafVisitorBounds();
    v->setAbsolutePoseFlag(true);
//    mafObjectRegistry::instance()->applyVisitorToObjectListThreaded(v, &m_VisibleVMEsList);
    mafObjectRegistry::instance()->applyVisitorToObjectList(v, &m_VisibleVMEsList);    
    mafBounds *bounds = v->bounds();
    
    double b[6] = {0.,0.,0.,0.,0.,0.};
    if(bounds) {
        bounds->bounds(b);
    }
    m_GUI->setBounds(b);
    double *p = (double *)m_GUI->position();

    /// set origin
    m_SlicePosition->setPosition(p);
    
    if (show) {
        mafView *subView;
        QList<mafView *> *vList = viewList();
        Q_FOREACH(subView, *vList) {
            mafSceneNode *sub_node = subView->sceneNodeFromVme((mafCore::mafObjectBase *)vme);
            mafPipeVisual *vp = sub_node->visualPipe();
            if (vp) {
                vp->setDelegateObject(this);
                bool res = connect(this, SIGNAL(modifiedObject()), vp, SLOT(updatePipe()));
                vp->updatePipe();
            }
        }
        
        //creation of the widgets
        if (m_OrthoPlaneTool.isEmpty()) {
            addPlaneToolsToHandler();
        }
    } 
    
    Q_FOREACH(mafToolVTKOrthoPlane *op, m_OrthoPlaneTool) {
        op->setSceneNode(node);
        op->setVOI(bounds);
        op->setVisibility(m_VisibleObjects != 0);
        op->setOrigin(m_SlicePosition);
    }
    mafDEL(v);
    updateView();
}

//////////////////////////////////////////////////////////////////////////
// Delegate methods
//////////////////////////////////////////////////////////////////////////

mafPointPointer mafViewOrthoSlice::origin() {
    return m_SlicePosition;
}

bool mafViewOrthoSlice::shouldExecuteLocalCode() {
    return false;
}
