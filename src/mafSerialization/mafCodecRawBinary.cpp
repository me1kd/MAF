/*
 *  mafCodecRawBinary.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecRawBinary.h"
#include <mafEventBusManager.h>
#include "QDir"

using namespace mafCore;
using namespace mafSerialization;
using namespace mafEventBus;

mafCodecRawBinary::mafCodecRawBinary(const QString code_location) : mafCodecRaw(code_location) {
    m_EncodingType = "RAW";
}

mafCodecRawBinary::~mafCodecRawBinary() {
}

void mafCodecRawBinary::encode(mafMemento *memento) {
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);

    m_DataStreamWrite.setDevice(m_Device);
    m_DataStreamWrite.setVersion(QDataStream::Qt_4_6);

    QString path = ((QFile *) m_Device)->fileName().section('/', 0, -2);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    const QMetaObject* meta = memento->metaObject();
    m_DataStreamWrite << QString("MementoType");
    m_DataStreamWrite << m_LevelEncode;
    QString mementoType = meta->className();
    m_DataStreamWrite << mementoType;
    QString ot = memento->objectClassType();
    m_DataStreamWrite << ot;

    foreach(item, *propList) { //for cycle should be inside each encodeItem
      m_DataStreamWrite << item.m_Name;
      m_DataStreamWrite << (int)item.m_Multiplicity;
      marshall(item.m_Value); //If will be removed: each memento will have its "encodeItem", and marshall will be moved in a separated class

      if (mementoType == "mafResources::mafMementoDataSet") {
        // use mafMementoDataSet to encode dataSet items.
        memento->encodeItem(&item, path);
      }
    }

    QObject *obj;
    ++m_LevelEncode;
    foreach(obj, memento->children()) {
      this->encode((mafMemento *)obj);
    }
    --m_LevelEncode;
}

mafMemento *mafCodecRawBinary::decode() {
    REQUIRE(m_Device != NULL);

    QString mementoTagSeparator;
    QString mementoType;
    QString objType;
    if(m_LevelDecode == -1) {  
      //-1 for initializing the file
      m_DataStreamRead.setDevice(m_Device);
      m_DataStreamRead >> mementoTagSeparator;
    }
    QString path = ((QFile *) m_Device)->fileName().section('/', 0, -2);
    m_DataStreamRead >> m_LevelDecode; //read memento level
    m_DataStreamRead >> mementoType;
    m_DataStreamRead >> objType;

    mafMemento *memento = (mafMemento *)mafNEWFromString(mementoType);
    memento->setObjectClassType(objType);

    //Fill the map of memento and levelDecode.
    m_MementoMap[m_LevelDecode] = memento;
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    while(!m_DataStreamRead.atEnd()) {
        m_DataStreamRead >> mementoTagSeparator;
        if(mementoTagSeparator != "MementoType") {
            item.m_Name = mementoTagSeparator;
            if (mementoTagSeparator.isEmpty()) {
                continue;
            } else {
                m_DataStreamRead >> item.m_Multiplicity;
            }
            QString typeName;
            m_DataStreamRead >> typeName;
            item.m_Value = demarshall(typeName, item.m_Multiplicity);
            if(mementoType == "mafResources::mafMementoDataSet") {
              // use mafMementoDataSet to encode dataSet items.
              memento->decodeItem(&item, path);
            } 
            propList->append(item);
        } else {
            int parentLevel = m_LevelDecode;
            mafMemento *mChild = decode();
            int parentRelation = m_LevelDecode - parentLevel;
            if (parentRelation > 0) {
              mChild->setParent(memento);
            } else {
              QMap<int, mafMemento*>::const_iterator i = m_MementoMap.find(m_LevelDecode-1);
              mafMemento *mementoParent = (mafMemento*)i.value();
              mChild->setParent(mementoParent);
            }
            m_LevelDecode = parentLevel;
        }
    }

    return memento;
}

void mafCodecRawBinary::marshall(const QVariant &value ){
    switch( value.type() ){
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
                m_DataStreamWrite << QString("int");
                m_DataStreamWrite << (int)value.toInt();
                break;
        case QVariant::Double:
                m_DataStreamWrite << QString("double");
                m_DataStreamWrite << (double)value.toDouble();
                break;
        case QVariant::Bool:
                m_DataStreamWrite << QString("boolean");
                m_DataStreamWrite << (bool)value.toBool();
                break;
        case QVariant::Date:
                m_DataStreamWrite << QString("dateTime.iso8601");
                m_DataStreamWrite << (QString)value.toDate().toString( Qt::ISODate );
                break;
        case QVariant::DateTime:
                m_DataStreamWrite << QString("dateTime.iso8601");
                m_DataStreamWrite << (QString)value.toDateTime().toString( Qt::ISODate );
                break;
        case QVariant::Time:
                m_DataStreamWrite << QString("dateTime.iso8601");
                m_DataStreamWrite << (QString)value.toTime().toString( Qt::ISODate );
                break;
        case QVariant::StringList:
        case QVariant::List: {
                m_DataStreamWrite << QString("list");
                foreach( QVariant item, value.toList() ) {
                        marshall( item );
                    }
                break;
        }
        case QVariant::Map: {
            QMap<QString, QVariant> map = value.toMap();
            QMap<QString, QVariant>::ConstIterator index = map.begin();
            m_DataStreamWrite << QString("map");
            while( index != map.end() ) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                marshall( *index );
                ++index;
            }
            break;
        }
        case QVariant::Hash: {
            QHash<QString, QVariant> hash = value.toHash();
            QHash<QString, QVariant>::ConstIterator index = hash.begin();
            m_DataStreamWrite << QString("hash");
            while( index != hash.end() ) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                m_DataStreamWrite << multi;
                marshall( *index );
                ++index;
            }
            break;
        }
        case QVariant::ByteArray: {
            m_DataStreamWrite << QString("base64");
            m_DataStreamWrite << value.toByteArray().toBase64();
            break;
        }
        default: {
            if( value.canConvert(QVariant::String) ) {
                m_DataStreamWrite << QString("string");
                m_DataStreamWrite << (QString)value.toString();
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

QVariant mafCodecRawBinary::demarshall( QString typeName, int multiplicity ) {
    if ( typeName == "string" ) {
        QString value;
        m_DataStreamRead >> value;
        return QVariant( value );
    } else if (typeName == "int") {
        int value = 0;
        m_DataStreamRead >> value;
        QVariant val( value );
        return val;
    } else if( typeName == "double" ) {
        double value = 0;
        m_DataStreamRead >> value;
        QVariant val( value );
        return val;
    } else if( typeName == "boolean" ) {
        bool value;
        m_DataStreamRead >> value;
        return QVariant( value );
    } else if( typeName == "datetime" || typeName == "dateTime.iso8601" ) {
        QString value;
        m_DataStreamRead >> value;
        return QVariant( QDateTime::fromString( value, Qt::ISODate ) );
    } else if( typeName == "list" ) {
        QVariantList value;

        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
            value.append(QVariant(demarshall(type, multi)));
        }
        return QVariant( value );
    } else if( typeName == "map" )
    {
        QMap<QString,QVariant> stct;

        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
            QString nodeName = demarshall( type, multi ).toString();
            m_DataStreamRead >> type;
            m_DataStreamRead >> multi;
            stct[ nodeName ] = QVariant(demarshall( type, multi ));
        }
        return QVariant(stct);
    } else if( typeName == "hash" )
    {
        QHash<QString,QVariant> hash;
        int i = 0;
        for (; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
            QString nodeName = demarshall( type, multi ).toString();
            m_DataStreamRead >> multi;
            m_DataStreamRead >> type;
            hash[ nodeName ] = QVariant(demarshall( type, multi ));
        }
        return QVariant(hash);
    } else if( typeName == "base64" ) {
        QVariant returnVariant;
        QByteArray dest;
        QByteArray src;
        m_DataStreamRead >> src;
        dest = QByteArray::fromBase64( src );
        QDataStream ds(&dest, QIODevice::ReadOnly);
        ds.setVersion(QDataStream::Qt_4_6);
        ds >> returnVariant;
        if( returnVariant.isValid() ) {
            return returnVariant;
        } else {
            return QVariant( dest );
        }
    }
    qCritical() << QString( "Cannot handle type %1").arg(typeName);
    return QVariant();
}


