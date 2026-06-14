#pragma once

#include <QWidget>
#include "MatrixManager.h"

class GridWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GridWidget(QWidget* parent = nullptr);

    // Set the matrix pointer to render. Does not take ownership.
    void setMatrix(const MatrixManager* matrix);

signals:
    // Signal emitted when a cell is clicked, passing its row and column
    void cellClicked(int row, int col);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    const MatrixManager* m_matrix;
};
