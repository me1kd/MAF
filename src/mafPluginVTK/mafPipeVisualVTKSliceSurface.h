/*
 *  mafPipeVisualVTKSliceSurface.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 09/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEVISUALVTKSLICESURFACE_H
#define MAFPIPEVISUALVTKSLICESURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"

// Class forwarding list
class vtkPlane;
class vtkCutter;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKSliceSurface
 This visual pipe allow to extract a contour from a surface data given a cut origin.
*/
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSliceSurface : public mafPipeVisualVTK {
    Q_OBJECT
    Q_PROPERTY(QString originX READ originX WRITE setOriginX)
    Q_PROPERTY(QString originY READ originY WRITE setOriginY)
    Q_PROPERTY(QString originZ READ originZ WRITE setOriginZ)
    Q_PROPERTY(QString thickness READ thickness WRITE setThickness)

    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKSliceSurface(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Set the origin of the slice
    void SetSlice(double *origin);

    /// Set the normal of the slice
    void SetNormal(double *normal);

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Set originX value from text box.
    void on_originX_textEdited(QString stringValue);

    /// Set originY value from text box.
    void on_originY_textEdited(QString stringValue);

    /// Set originZ value from text box.
    void on_originZ_textEdited(QString stringValue);

    /// Set thickness value from text box.
    void on_thickness_textEdited(QString stringValue);

    /// Get originX value;
    QString originX();

    /// Set originX value;
    void setOriginX(QString value);

    /// Get originY value;
    QString originY();

    /// Set originY value;
    void setOriginY(QString value);

    /// Get originZ value;
    QString originZ();

    /// Set originZ value;
    void setOriginZ(QString value);

    /// Get thickness value;
    QString thickness();

    /// Set thickness value;
    void setThickness(QString value);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSliceSurface();

private:
    double m_SliceOrigin[3]; ///< Origin of the slice.
    double m_Normal[3]; ///< Normal vector of the slice.
    double m_Thickness; ///< Contour thickness.

    vtkCutter *m_Cutter;    ///< Cutter fo the input vtkPolyData.
    vtkPlane  *m_Plane;     ///< Cutting plane implicit function.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QString mafPipeVisualVTKSliceSurface::originX() {
    return QString::number(m_SliceOrigin[0]);
}

inline void mafPipeVisualVTKSliceSurface::setOriginX(QString stringValue) {
    m_SliceOrigin[0] = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_originX_textEdited(QString stringValue) {
    m_SliceOrigin[0] = stringValue.toDouble();
}

inline QString mafPipeVisualVTKSliceSurface::originY() {
    return QString::number(m_SliceOrigin[1]);
}

inline void mafPipeVisualVTKSliceSurface::setOriginY(QString stringValue) {
    m_SliceOrigin[1] = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_originY_textEdited(QString stringValue) {
    m_SliceOrigin[1] = stringValue.toDouble();
}

inline QString mafPipeVisualVTKSliceSurface::originZ() {
    return QString::number(m_SliceOrigin[2]);
}

inline void mafPipeVisualVTKSliceSurface::setOriginZ(QString stringValue) {
    m_SliceOrigin[2] = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_originZ_textEdited(QString stringValue) {
    m_SliceOrigin[2] = stringValue.toDouble();
}

inline QString mafPipeVisualVTKSliceSurface::thickness() {
    return QString::number(m_Thickness);
}

inline void mafPipeVisualVTKSliceSurface::setThickness(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

inline void mafPipeVisualVTKSliceSurface::on_thickness_textEdited(QString stringValue) {
    m_Thickness = stringValue.toDouble();
}

} // namespace mafPluginVTK

#endif // MAFPIPEVISUALVTKSLICESURFACE_H