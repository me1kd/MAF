/*
 *  mafEvent.cpp
 *  mafEventBus
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEvent.h"

using namespace mafEventBus;

mafEvent::mafEvent() : m_SynchronousEvent(true) {
    m_EventHash = new QVariantHash();
}

/// Overload object constructor.
mafEvent::mafEvent(QString topic, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, QString signature, bool synchronous) : m_SynchronousEvent(synchronous) {
    m_EventHash = new QVariantHash();
    entries()->insert(TOPIC, topic);
    entries()->insert(TYPE, static_cast<int>(event_type));
    entries()->insert(SIGTYPE, static_cast<int>(signature_type));
    QVariant var;
    var.setValue(objectPointer);
    entries()->insert(OBJECT, var);
    entries()->insert(SIGNATURE, signature);
}

mafEvent::mafEvent(QString topic, mafEventType event_type, bool synchronous) : m_SynchronousEvent(synchronous) {
    m_EventHash = new QVariantHash();
    entries()->insert(TOPIC, topic);
    entries()->insert(TYPE, static_cast<int>(event_type));
}

mafEvent::~mafEvent() {
    m_EventHash->clear();
    m_ArgList.clear();
    delete m_EventHash;
}

QVariantHash *mafEvent::entries() {
    return m_EventHash;
}

QVariantHash *mafEvent::entries() const {
    return m_EventHash;
}

QVariant &mafEvent::operator[](QString key) const{
    return (*m_EventHash)[key];
}

mafEventType mafEvent::eventType() const {
    return static_cast<mafEventType>(entries()->value(TYPE).toInt());
}

QString mafEvent::eventTopic() const {
    return entries()->value(TOPIC).toString();
}

bool mafEvent::isEventLocal() const {
    int et = entries()->value(TYPE).toInt();
    return et == mafEventTypeLocal;
}

void mafEvent::setEventType(mafEventType et) {
    entries()->insert(TYPE, static_cast<int>(et));
}

void mafEvent::setEventTopic(QString topic) {
    entries()->insert(TOPIC, topic);
}

void mafEvent::clearArgList() {
    m_ArgList.clear();
}

/*QString mafEvent::eventIdName() const {
    mafId id = eventId();
    return mafIdProvider::instance()->idName(id);
}*/
