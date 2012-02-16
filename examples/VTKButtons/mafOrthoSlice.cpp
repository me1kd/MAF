/*
 *  mafOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 09/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOrthoSlice.h"
#include "ui_mafOrthoSlice.h"

#include <QDebug>

mafOrthoSlice::mafOrthoSlice(QWidget *parent) : QWidget(parent), ui(new Ui::mafOrthoSlice), m_Decimals(2) {
    ui->setupUi(this);
    ui->xPosition->setEnabled(false);
    ui->yPosition->setEnabled(false);
    ui->zPosition->setEnabled(false);
    m_Bounds[0] = 0.;
    m_Bounds[1] = 1.;
    m_Bounds[2] = 0.;
    m_Bounds[3] = 1.;
    m_Bounds[4] = 0.;
    m_Bounds[5] = 1.;
    m_Position[0] = m_Position[1] = m_Position[2] = 0.;
}

mafOrthoSlice::~mafOrthoSlice() {
    delete ui;
}

void mafOrthoSlice::setBounds(double bounds[6]) {
    double scaledBound;
    int sliderBounds[6];
    int multiplier = 1;
    for (int i = 0; i < 6; ++i) {
        m_Bounds[i] = bounds[i];
        scaledBound = bounds[i];
        for (int d = 0; d < m_Decimals; ++d) {
            multiplier *= 10;
            scaledBound *= 10;
        }
        sliderBounds[i] = (int)scaledBound;
    }

    ui->xPosition->setEnabled(sliderBounds[0] < sliderBounds[1]);
    ui->yPosition->setEnabled(sliderBounds[2] < sliderBounds[3]);
    ui->zPosition->setEnabled(sliderBounds[4] < sliderBounds[5]);

    // update the slider's range and value according to the new bounds.
    ui->xPosition->setRange(sliderBounds[0], sliderBounds[1]);
    ui->yPosition->setRange(sliderBounds[2], sliderBounds[3]);
    ui->zPosition->setRange(sliderBounds[4], sliderBounds[5]);

    m_Position[0] = (m_Bounds[0] + m_Bounds[1]) / 2;
    m_Position[1] = (m_Bounds[2] + m_Bounds[3]) / 2;
    m_Position[2] = (m_Bounds[4] + m_Bounds[5]) / 2;

    ui->xPosition->setValue((sliderBounds[0] + sliderBounds[1]) / 2);
    ui->yPosition->setValue((sliderBounds[2] + sliderBounds[3]) / 2);
    ui->zPosition->setValue((sliderBounds[4] + sliderBounds[5]) / 2);

    ui->xPosition->setTickInterval((sliderBounds[1] - sliderBounds[0]) / multiplier);
    ui->yPosition->setTickInterval((sliderBounds[3] - sliderBounds[2]) / multiplier);
    ui->zPosition->setTickInterval((sliderBounds[5] - sliderBounds[4]) / multiplier);
}

void mafOrthoSlice::setPosition(double pos[3]) {
    m_Position[0] = pos[0];
    m_Position[1] = pos[1];
    m_Position[2] = pos[2];
    ui->xPosition->setValue(m_Position[0]);
    ui->yPosition->setValue(m_Position[1]);
    ui->zPosition->setValue(m_Position[2]);
}

void mafOrthoSlice::setDecimalDigits(int decimal) {
    m_Decimals = decimal;

    // Update ranges and values of position sliders.
    this->setBounds(m_Bounds);
}

void mafOrthoSlice::on_xPosition_sliderMoved(int val) {
    m_Position[0] = this->calculateRealValue(val);
//    qDebug() << QString("X: %1").arg(m_Position[0]);
    Q_EMIT positionUpdated(m_Position);
}

void mafOrthoSlice::on_yPosition_sliderMoved(int val) {
    m_Position[1] = this->calculateRealValue(val);
//    qDebug() << QString("Y: %1").arg(m_Position[1]);
    Q_EMIT positionUpdated(m_Position);
}

void mafOrthoSlice::on_zPosition_sliderMoved(int val) {
    m_Position[2] = this->calculateRealValue(val);
//    qDebug() << QString("Z: %1").arg(m_Position[2]);
    Q_EMIT positionUpdated(m_Position);
}

double mafOrthoSlice::calculateRealValue(int val) {
    double scaledVal = val;
    for (int d = 0; d < m_Decimals; ++d) {
        scaledVal /= 10.;
    }
    return scaledVal;
}
