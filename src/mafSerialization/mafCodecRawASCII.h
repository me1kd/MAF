/*
 *  mafCodecRawASCII.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani and Roberto Mucci on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafCodecRawASCII_H
#define mafCodecRawASCII_H

// Includes list
#include "mafCodecRaw.h"

namespace mafSerialization {

/**
 Class name: mafCodecRawASCII
 Codec to Encode/Decode maf objects into an ASCII raw memory dump.
 @sa mafCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafCodecRawASCII : public mafCodecRaw {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCodecRaw);

public:
    /// Object constructor.
    mafCodecRawASCII(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafCore::mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafCore::mafMemento *decode();

protected:
    /// Object destructor.
    /* virtual */ ~mafCodecRawASCII();

private:
    /// marshalling of QVariant data type in order to write the correct type.
    void marshall(const QVariant &value );

    /// unmarshalling of QVariant data type in order to write the correct type.
    QVariant demarshall( QString typeName, int multiplicity);

    QTextStream m_DataTextRead;
    QTextStream m_DataTextWrite;
};

} // mafSerialization

#endif // mafCodecRawASCII_H
