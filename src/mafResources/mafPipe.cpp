/*
 *  mafPipe.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipe.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafPipe::mafPipe(const QString code_location) : mafObject(code_location), m_InputList(NULL) {
    m_InputList = new mafVMEList();
}

mafPipe::~mafPipe() {
    int i = 0, size = m_InputList->size();
    for(;i<size;++i) {
        m_InputList->at(i)->release();
    }
    m_InputList->clear();
    
    delete m_InputList;
    m_InputList = NULL;
}

void mafPipe::setInput(mafVME *vme) {
    REQUIRE(vme != NULL);
    if(!m_InputList->isEmpty()) {
        removeInput(0);
    }
    m_InputList->append(vme);
    vme->retain();

    mafDataSetCollection *datSetCollection = vme->dataSetCollection();
    if (datSetCollection) {
      //connect the data collection modified to the updatePipe slot
      connect(datSetCollection, SIGNAL(modifiedObject()), this, SLOT(updatePipe()));
    }
    setModified();
}

void mafPipe::removeInput(mafVME *vme) {
    REQUIRE(vme != NULL);

    int idx = m_InputList->indexOf(vme);
    if(idx != -1) {
        m_InputList->removeAt(idx);
        vme->release();
    } else {
        qWarning("%s", mafTr("Object %1 not present in input list").arg(vme->objectName()).toAscii().data());
    }
}

void mafPipe::removeInput(const int idx) {
    REQUIRE(idx >= 0);

    if(idx < m_InputList->count()) {
        mafVME *vme = m_InputList->at(idx);
        m_InputList->removeAt(idx);
        vme->release();
    } else {
        qWarning("%s", mafTr("Index %1 outside input list range.").arg(idx).toAscii().data());
    }
}
