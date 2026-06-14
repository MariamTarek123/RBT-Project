#pragma once

#include <QMainWindow>
#include <memory>
#include "MatrixManager.h"

class QSpinBox;
class QPushButton;
class QLabel;
class GridWidget;
class TreeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

    // Core GUI operations requested
    void createMatrix(int rows, int cols);
    void clearMatrix();
    void updateUI();
    void drawGrid();
    void drawTree();
    bool isCellOne(int row, int col) const;

private slots:
    void onCellClicked(int row, int col);
    void onCreateClicked();
    void onClearClicked();

private:
    // Safe memory management of backend instance
    std::unique_ptr<MatrixManager> m_matrix;

    // UI Controls
    QSpinBox* m_rowsSpinBox;
    QSpinBox* m_colsSpinBox;
    QPushButton* m_createButton;
    QPushButton* m_clearButton;
    QLabel* m_onesCountLabel;
    
    // Zoom Controls
    QPushButton* m_zoomInButton;
    QPushButton* m_zoomOutButton;
    QPushButton* m_resetZoomButton;

    // Splitted Panes
    GridWidget* m_gridWidget;
    TreeView* m_treeView;
};
