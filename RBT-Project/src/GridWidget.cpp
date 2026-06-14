#include "GridWidget.h"
#include <QPainter>
#include <QMouseEvent>

GridWidget::GridWidget(QWidget* parent)
    : QWidget(parent)
    , m_matrix(nullptr)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void GridWidget::setMatrix(const MatrixManager* matrix)
{
    m_matrix = matrix;
    update(); // Trigger repaint
}

void GridWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    
    if (!m_matrix) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "No matrix initialized. Enter size and click 'Create'.");
        return;
    }

    int rows = m_matrix->getRows();
    int cols = m_matrix->getCols();
    if (rows <= 0 || cols <= 0) return;

    double cellWidth = static_cast<double>(width()) / cols;
    double cellHeight = static_cast<double>(height()) / rows;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QRectF cellRect(c * cellWidth, r * cellHeight, cellWidth, cellHeight);
            
            // 1 is Sea Green (#2E8B57), 0 is White
            QColor cellColor = m_matrix->isCellOne(r, c) ? QColor("#2E8B57") : Qt::white;
            painter.fillRect(cellRect, cellColor);

            // More visible border line with thickness 2
            painter.setPen(QPen(QColor(160, 160, 160), 2));
            painter.drawRect(cellRect);
        }
    }
}

void GridWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_matrix) return;

    int rows = m_matrix->getRows();
    int cols = m_matrix->getCols();
    if (rows <= 0 || cols <= 0) return;

    double cellWidth = static_cast<double>(width()) / cols;
    double cellHeight = static_cast<double>(height()) / rows;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    double clickX = event->position().x();
    double clickY = event->position().y();
#else
    double clickX = event->x();
    double clickY = event->y();
#endif

    int col = static_cast<int>(clickX / cellWidth);
    int row = static_cast<int>(clickY / cellHeight);

    // Safe bounds check
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        emit cellClicked(row, col);
    }
}
