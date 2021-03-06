/*
 *  mafDiagramScene.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 11/02/13.
 *  Copyright 2013 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDiagramScene.h"

using namespace mafGUI;

mafDiagramScene::mafDiagramScene(QMenu *itemMenu, QObject *parent) : QGraphicsScene(parent)
{
    mItemMenu = itemMenu;
    mMode = MoveItem;
    mLineColor = Qt::black;
    tmpArrow = NULL;
    tmpConnector = NULL;
    lastHighlighted = NULL;
    mDebugDraw = false;
}

QColor mafDiagramScene::lineColor() const {
    return mLineColor;
}

void mafDiagramScene::insertNode(mafNodeGraphicWidget *node) {
    emit mafNodeGraphicWidgetInserted(node);
}

void mafDiagramScene::insertArrow(mafNodeConnectionGraphicWidget *arrow) {
    arrow->updatePosition();
    arrow->update();
}

void mafDiagramScene::setLineColor(const QColor &color)
{
    mLineColor = color;
/*    if (isItemChange(Arrow::Type)) {
        Arrow *item =
            qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(mLineColor);
        update();
    }
	*/
}

void mafDiagramScene::setMode(Mode mode)
{
    mMode = mode;
}

void mafDiagramScene::setMenu(QMenu *menu) {
    mItemMenu = menu;
}

QMenu *mafDiagramScene::menu() {
    return mItemMenu;
}

void mafDiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton) {
		QList<QGraphicsItem *> startConnectors = items(mouseEvent->scenePos());
		foreach(QGraphicsItem * g, startConnectors) {
                        if (g->type() == mafNodeConnectorGraphicWidget::Type) {
                                mafNodeConnectorGraphicWidget* start = dynamic_cast<mafNodeConnectorGraphicWidget*>(g);

				if (start->mSingleConnection) {
					start->deleteConnections();
				}

                                tmpConnector = new mafNodeConnectorGraphicWidget(NULL, this, NULL,mafNodeConnectorGraphicWidget::InOut);
				tmpConnector->setPos(mouseEvent->scenePos());

				if (isDebugDraw()) {
					//debug draw
					tmpConnector->mRadius = 5;
				}
				else {
					tmpConnector->setVisible(false);
					tmpConnector->mRadius = 0;
				}


                                if (start->connectorType() != mafNodeConnectorGraphicWidget::In) {
                                        tmpArrow = new mafGUI::mafNodeConnectionGraphicWidget(start, tmpConnector, NULL, this);
				}
				else {
                                        tmpArrow = new mafGUI::mafNodeConnectionGraphicWidget(tmpConnector, start, NULL, this);
				}

				start->setHighlight(true);
				existingConnector = start;

				
				existingConnector->updatePosition();
				existingConnector->update();
				tmpConnector->updatePosition();
				tmpConnector->update();
				tmpArrow->updatePosition();
				tmpArrow->update();

                                //QGraphicsScene::mousePressEvent(mouseEvent);
				return;
			}
		}
	}

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void mafDiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (tmpArrow != 0) {

		//simple right/left handling
		if (tmpArrow->startConnector()->mapToScene(0,0).x() < tmpArrow->endConnector()->mapToScene(0,0).x()) {
			if (tmpConnector == tmpArrow->endConnector())
                                tmpConnector->setConnectorAlignment(mafNodeConnectorGraphicWidget::Left);// right = false;
			else
				//tmpConnector->right = true;
                                tmpConnector->setConnectorAlignment(mafNodeConnectorGraphicWidget::Right);
		}
		else {
			if (tmpConnector == tmpArrow->endConnector())
				//tmpConnector->right = true;
                                tmpConnector->setConnectorAlignment(mafNodeConnectorGraphicWidget::Right);
			else
				//tmpConnector->right = false;
                                tmpConnector->setConnectorAlignment(mafNodeConnectorGraphicWidget::Left);
		}

		// if we hit a connecor, highlight it and take 'right' over, switch in and out if neccessary
		QList<QGraphicsItem *> intersectedItems = items(mouseEvent->scenePos());
                while(intersectedItems.count() && (intersectedItems.first() == tmpArrow || intersectedItems.first() == tmpConnector || intersectedItems.first()->type() != mafNodeConnectorGraphicWidget::Type)) {
            intersectedItems.removeFirst();
		}
		if (intersectedItems.count() && intersectedItems.first() != tmpConnector && intersectedItems.first() != existingConnector) {
			//dw thats how a cast should look like
                        mafNodeConnectorGraphicWidget *node = qgraphicsitem_cast<mafNodeConnectorGraphicWidget *>(intersectedItems.first());
			//ugly direction handling, only case where a switch is possible
                        if (existingConnector->connectorType() == mafNodeConnectorGraphicWidget::InOut) {
				//switch if non matching
                                if ((node->connectorType() == mafNodeConnectorGraphicWidget::Out && tmpArrow->startConnector() == existingConnector)
                                        || (node->connectorType() == mafNodeConnectorGraphicWidget::In && tmpArrow->endConnector() == existingConnector)) {
					//tmpConnector->setConnectorType(node->connectorType());
                                        mafNodeConnectorGraphicWidget* old1 = tmpArrow->startConnector();
                                        mafNodeConnectorGraphicWidget* old2 = tmpArrow->endConnector();
					//dw needed? removeItem(tmpArrow);
					//dw667 backmerge: to slow if connector is child of item
					//removeItem(tmpArrow);
					delete tmpArrow;
                                        tmpArrow = new mafNodeConnectionGraphicWidget(old2, old1, NULL, this);
					//dw667 backmerge: to slow if connector is child of item
					//addItem(tmpArrow);
				}
			}
			//check if uncompatible
                        else if (node->connectorType() != mafNodeConnectorGraphicWidget::InOut && node->connectorType() == existingConnector->connectorType()) {
				return;
			}

			node->setHighlight(true);
			/*node->updatePosition();
			node->update();*/
			//dw667 backmerge:
			node->updatePosition();
			node->update();
			
			tmpConnector->setConnectorAlignment(node->connectorAlignment());
			tmpConnector->setConnectorType(node->connectorType());
			//FIXME: make configurable
			tmpArrow->setBidirectional(true);

			tmpConnector->updatePosition();
			tmpConnector->update();
			lastHighlighted = node;
		}
		else if (lastHighlighted != NULL) {
			lastHighlighted->setHighlight(false);
			/*lastHighlighted->updatePosition();
			lastHighlighted->update();*/
			//dw667 backmerge:
			lastHighlighted->updatePosition();
			lastHighlighted->update();
			
			lastHighlighted = NULL;
			tmpConnector->updatePosition();
			tmpConnector->update();
		}

                //QLineF newLine(line->line().p1(), mouseEvent->scenePos());
		//line->setLine(newLine);
                //tmpConnector = new mafNodeConnectorGraphicWidget(NULL, scene, NULL, mafNodeConnectorGraphicWidget::InOut);
                //tmpConnector-> = new mafNodeConnectorGraphicWidget(NULL, scene, NULL, mafNodeConnectorGraphicWidget::InOut);
		tmpConnector->setPos(mouseEvent->scenePos());
		tmpArrow->updatePosition();
		tmpArrow->update();
		//dw667 backmerge: was commented
		tmpConnector->updatePosition();
		
		tmpConnector->update();
		return;
    }
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void mafDiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (tmpArrow != 0) {
                mafNodeConnectorGraphicWidget* startC = tmpArrow->startConnector();
                mafNodeConnectorGraphicWidget* endC = tmpArrow->endConnector();
		QPointF startPos(startC->mapToScene(0, 0));
		QPointF endPos(endC->mapToScene(0, 0));
		//QPointF endPos(mouseEvent->scenePos());

		tmpArrow->startConnector()->setHighlight(false);
		tmpArrow->endConnector()->setHighlight(false);
		/*tmpArrow->startConnector()->updatePosition();
		tmpArrow->startConnector()->update();
		tmpArrow->endConnector()->updatePosition();
		tmpArrow->endConnector()->update();*/

		QList<QGraphicsItem *> startConnectors = items(startPos);
                while(startConnectors.count() && (startConnectors.first() == tmpArrow || startConnectors.first() == tmpConnector || startConnectors.first()->type() != mafNodeConnectorGraphicWidget::Type)) {
            startConnectors.removeFirst();
		}
		QList<QGraphicsItem *> endConnectors = items(endPos);
                while(endConnectors.count() && (endConnectors.first() == tmpArrow || endConnectors.first() == tmpConnector || endConnectors.first()->type() != mafNodeConnectorGraphicWidget::Type)) {
            endConnectors.removeFirst();
		}

		//removeItem(tmpArrow);

        delete tmpArrow;
		tmpArrow = 0;
                //removeItem(tmpConnector);
        delete tmpConnector;
		//tmpConnector->deleteLater();
		tmpConnector = 0;
        
		if (startConnectors.count() > 0 && endConnectors.count() > 0 &&
            startConnectors.first()->type() == mafNodeConnectorGraphicWidget::Type &&
                        endConnectors.first()->type() == mafNodeConnectorGraphicWidget::Type &&
            startConnectors.first() != endConnectors.first())
		{
            mafNodeConnectorGraphicWidget *startConnector =
                qgraphicsitem_cast<mafNodeConnectorGraphicWidget *>(startConnectors.first());
            mafNodeConnectorGraphicWidget *endConnector =
                qgraphicsitem_cast<mafNodeConnectorGraphicWidget *>(endConnectors.first());

                        if (!((startConnector->connectorType() == mafNodeConnectorGraphicWidget::In && endConnector->connectorType() == mafNodeConnectorGraphicWidget::In) || (startConnector->connectorType() == mafNodeConnectorGraphicWidget::Out && endConnector->connectorType() == mafNodeConnectorGraphicWidget::Out)))
			{
                               mafNodeConnectionGraphicWidget *arrow = new mafNodeConnectionGraphicWidget(startConnector, endConnector, NULL, this);
				arrow->setColor(mLineColor);
				startConnector->addConnection(arrow);
				endConnector->addConnection(arrow);
				arrow->setZValue(-1000.0);

				arrow->startConnector()->setHighlight(false);
				arrow->endConnector()->setHighlight(false);
				arrow->startConnector()->update();
				arrow->endConnector()->update();
			}

			startConnector->setHighlight(false);
			endConnector->setHighlight(false);
                        //arrow->updatePosition();

			/*startConnector->updatePosition();
			startConnector->update();
			endConnector->updatePosition();
			endConnector->update();*/
        }
			//NOTE: assumtion that lastHighlighted is never deleted while drawing lines, otherwise pointer can go to nirvana (TODO: can we use this assumtion?)
		if (lastHighlighted != NULL) {
			lastHighlighted->setHighlight(false);
			/*lastHighlighted->updatePosition();
			lastHighlighted->update();*/
			lastHighlighted = NULL;
		}
		return;
    }

    //line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool mafDiagramScene::isItemChange(int type) {
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void mafDiagramScene::setDebugDraw(bool enabled) {
	mDebugDraw = enabled;
	this->update();
}
