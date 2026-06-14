#include "TreeView.h"
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QBrush>
#include <map>
#include <functional>

TreeView::TreeView(QWidget* parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
    , m_scaleFactor(1.0)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag); // Allow panning by clicking and dragging
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Premium dark theme styling for the view background
    setStyleSheet("QGraphicsView { background-color: #1c1c22; border: 1px solid #2d2d38; border-radius: 6px; }");
}

void TreeView::drawTree(RBNode* root, RBNode* NIL)
{
    m_scene->clear();

    if (!root || root == NIL) {
        QGraphicsSimpleTextItem* placeholder = m_scene->addSimpleText("RBTree is empty. Activate cells on the left.");
        placeholder->setBrush(QBrush(QColor("#a0a0b0")));
        QFont font = placeholder->font();
        font.setPointSize(10);
        font.setItalic(true);
        placeholder->setFont(font);
        
        // Center placeholder
        QRectF r = placeholder->boundingRect();
        placeholder->setPos(-r.width() / 2, -r.height() / 2);
        m_scene->setSceneRect(-r.width() / 2 - 20, -r.height() / 2 - 20, r.width() + 40, r.height() + 40);
        return;
    }

    // Map to store node coordinates
    std::map<RBNode*, QPointF> positions;
    int xIndex = 0;

    // 1. First Pass: In-order traversal to calculate X-coordinates based on rank,
    // and Y-coordinates based on node depth. This ensures zero overlaps.
    std::function<void(RBNode*, int)> calculatePositions = [&](RBNode* node, int depth) {
        if (!node || node == NIL) return;
        
        calculatePositions(node->left, depth + 1);
        
        double x = xIndex * HORIZONTAL_SPACING;
        double y = depth * VERTICAL_SPACING;
        positions[node] = QPointF(x, y);
        xIndex++;
        
        calculatePositions(node->right, depth + 1);
    };

    calculatePositions(root, 0);

    // 2. Second Pass: Draw lines (edges) and nodes
    std::function<void(RBNode*)> drawElements = [&](RBNode* node) {
        if (!node || node == NIL) return;

        QPointF pos = positions[node];

        // Draw edge to left child
        if (node->left && node->left != NIL) {
            QPointF childPos = positions[node->left];
            QGraphicsLineItem* line = m_scene->addLine(QLineF(pos, childPos));
            line->setPen(QPen(QColor("#5c5c6d"), 2));
            line->setZValue(0); // Under nodes
        }

        // Draw edge to right child
        if (node->right && node->right != NIL) {
            QPointF childPos = positions[node->right];
            QGraphicsLineItem* line = m_scene->addLine(QLineF(pos, childPos));
            line->setPen(QPen(QColor("#5c5c6d"), 2));
            line->setZValue(0); // Under nodes
        }

        // Draw node circle
        QGraphicsEllipseItem* circle = m_scene->addEllipse(
            pos.x() - NODE_RADIUS, pos.y() - NODE_RADIUS,
            2 * NODE_RADIUS, 2 * NODE_RADIUS
        );

        // Red nodes use #D9534F, black nodes use #222222
        QColor nodeColor = (node->color == RED) ? QColor("#D9534F") : QColor("#222222");
        circle->setBrush(QBrush(nodeColor));
        circle->setPen(QPen(QColor("#ffffff"), 1.5));
        circle->setZValue(1);

        // Add label (x,y)
        QGraphicsSimpleTextItem* label = m_scene->addSimpleText(QString("%1,%2").arg(node->x).arg(node->y));
        label->setBrush(QBrush(Qt::white));
        QFont font = label->font();
        font.setPointSize(8);
        font.setBold(true);
        label->setFont(font);

        // Center the label text inside the node circle
        QRectF textRect = label->boundingRect();
        label->setPos(pos.x() - textRect.width() / 2.0, pos.y() - textRect.height() / 2.0);
        label->setZValue(2); // Top-most layer

        // Recurse to children
        drawElements(node->left);
        drawElements(node->right);
    };

    drawElements(root);

    // Update scene rect to fit all items nicely
    m_scene->setSceneRect(m_scene->itemsBoundingRect().adjusted(-50, -50, 50, 50));
}

void TreeView::zoomIn()
{
    scale(1.15, 1.15);
    m_scaleFactor *= 1.15;
}

void TreeView::zoomOut()
{
    scale(1.0 / 1.15, 1.0 / 1.15);
    m_scaleFactor /= 1.15;
}

void TreeView::resetZoom()
{
    resetTransform();
    m_scaleFactor = 1.0;
}

void TreeView::wheelEvent(QWheelEvent* event)
{
    // Zoom on wheel scroll
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}
