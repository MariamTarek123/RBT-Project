#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include "RBNode.h"

class TreeView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TreeView(QWidget* parent = nullptr);

    // Clears the scene and redraws the Red-Black Tree
    void drawTree(RBNode* root, RBNode* NIL);

    // Zoom controls
    void zoomIn();
    void zoomOut();
    void resetZoom();

protected:
    // Support mouse wheel zooming (with Ctrl modifier or by default)
    void wheelEvent(QWheelEvent* event) override;

private:
    QGraphicsScene* m_scene;
    double m_scaleFactor;

    // Helper spacing constants
    static constexpr double HORIZONTAL_SPACING = 55.0;
    static constexpr double VERTICAL_SPACING = 65.0;
    static constexpr double NODE_RADIUS = 20.0;
};
