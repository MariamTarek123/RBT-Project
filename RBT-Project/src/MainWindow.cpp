#include "MainWindow.h"
#include "GridWidget.h"
#include "TreeView.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>
#include <QFrame>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Apply premium dark stylesheet for maximum elegance
    setStyleSheet(R"(
        QMainWindow {
            background-color: #121216;
        }
        QWidget {
            color: #e2e2e9;
            font-family: "Segoe UI", "Arial", sans-serif;
            font-size: 12pt;
        }
        QFrame#controlWidget {
            background-color: #1a1a22;
            border-bottom: 2px solid #282834;
        }
        QLabel {
            font-weight: bold;
        }
        QLabel#onesCount {
            color: #2E8B57;
            font-size: 14pt;
            margin-left: 10px;
        }
        QSpinBox {
            background-color: #24242e;
            border: 1px solid #3d3d4f;
            border-radius: 4px;
            padding: 4px 8px;
            color: #ffffff;
            font-weight: bold;
        }
        QSpinBox:focus {
            border: 1px solid #00a896;
        }
        QPushButton {
            background-color: #2e2e3c;
            border: 1px solid #4a4a60;
            border-radius: 4px;
            padding: 6px 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3f3f52;
            border-color: #60607e;
        }
        QPushButton:pressed {
            background-color: #20202a;
        }
        QPushButton#createBtn {
            background-color: #00a896;
            border: none;
            color: white;
        }
        QPushButton#createBtn:hover {
            background-color: #02c39a;
        }
        QPushButton#createBtn:pressed {
            background-color: #028090;
        }
        QPushButton#clearBtn {
            background-color: #c94a4a;
            border: none;
            color: white;
        }
        QPushButton#clearBtn:hover {
            background-color: #e05c5c;
        }
        QPushButton#clearBtn:pressed {
            background-color: #a63535;
        }
        QStatusBar {
            background-color: #1a1a22;
            border-top: 1px solid #282834;
            color: #a0a0b0;
        }
        QSplitter::handle {
            background-color: #282834;
        }
        QSplitter::handle:horizontal {
            width: 4px;
        }
    )");

    // Central widget & layout setup
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Title label at the top center
    QLabel* titleLabel = new QLabel("Matrix Red Black Tree", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 28pt;
            font-weight: bold;
            color: #00a896;
            background-color: #1a1a22;
            padding: 12px 0px 4px 0px;
        }
    )");
    mainLayout->addWidget(titleLabel);

    // Top control panel
    QFrame* controlPanel = new QFrame(this);
    controlPanel->setObjectName("controlWidget");
    QHBoxLayout* controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(12, 10, 12, 10);
    controlLayout->setSpacing(10);

    QLabel* rowsLabel = new QLabel("Rows:", this);
    m_rowsSpinBox = new QSpinBox(this);
    m_rowsSpinBox->setRange(1, 200);
    m_rowsSpinBox->setValue(5);

    QLabel* colsLabel = new QLabel("Cols:", this);
    m_colsSpinBox = new QSpinBox(this);
    m_colsSpinBox->setRange(1, 200);
    m_colsSpinBox->setValue(5);

    m_createButton = new QPushButton("Create", this);
    m_createButton->setObjectName("createBtn");

    m_clearButton = new QPushButton("Clear", this);
    m_clearButton->setObjectName("clearBtn");

    m_onesCountLabel = new QLabel("Ones Count: 0", this);
    m_onesCountLabel->setObjectName("onesCount");

    // Add items to layout
    controlLayout->addWidget(rowsLabel);
    controlLayout->addWidget(m_rowsSpinBox);
    controlLayout->addWidget(colsLabel);
    controlLayout->addWidget(m_colsSpinBox);
    controlLayout->addWidget(m_createButton);
    controlLayout->addWidget(m_clearButton);
    controlLayout->addWidget(m_onesCountLabel);
    
    controlLayout->addStretch(1); // Push zoom controls to the right

    QLabel* zoomLabel = new QLabel("Zoom Tree:", this);
    m_zoomInButton = new QPushButton("+", this);
    m_zoomInButton->setFixedWidth(30);
    m_zoomOutButton = new QPushButton("-", this);
    m_zoomOutButton->setFixedWidth(30);
    m_resetZoomButton = new QPushButton("Reset", this);

    controlLayout->addWidget(zoomLabel);
    controlLayout->addWidget(m_zoomInButton);
    controlLayout->addWidget(m_zoomOutButton);
    controlLayout->addWidget(m_resetZoomButton);

    mainLayout->addWidget(controlPanel);

    // Horizontal splitter for Grid and Tree views
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setContentsMargins(10, 10, 10, 10);
    
    // Left pane
    m_gridWidget = new GridWidget(this);
    splitter->addWidget(m_gridWidget);

    // Right pane
    m_treeView = new TreeView(this);
    splitter->addWidget(m_treeView);

    // Equal default sizes
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);

    mainLayout->addWidget(splitter, 1); // Add stretch to push title up

    // Bottom Status Bar
    QStatusBar* statusBarObj = new QStatusBar(this);
    setStatusBar(statusBarObj);

    // Wire up events
    connect(m_createButton, &QPushButton::clicked, this, &MainWindow::onCreateClicked);
    connect(m_clearButton, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(m_gridWidget, &GridWidget::cellClicked, this, &MainWindow::onCellClicked);
    
    // Tree zoom events
    connect(m_zoomInButton, &QPushButton::clicked, m_treeView, &TreeView::zoomIn);
    connect(m_zoomOutButton, &QPushButton::clicked, m_treeView, &TreeView::zoomOut);
    connect(m_resetZoomButton, &QPushButton::clicked, m_treeView, &TreeView::resetZoom);

    // Build the initial 5x5 matrix
    createMatrix(5, 5);

    setWindowTitle("Sparse Matrix Red-Black Tree Visualizer");
    resize(950, 650);
}

void MainWindow::createMatrix(int rows, int cols)
{
    if (rows <= 0 || cols <= 0) {
        QMessageBox::warning(this, "Dimensions Error", "Matrix size must be greater than zero.");
        return;
    }

    // Safely reinitialize the matrix (prevents dangling pointer leaks)
    m_matrix = std::make_unique<MatrixManager>(rows, cols);

    // Pass new pointer to GridWidget
    m_gridWidget->setMatrix(m_matrix.get());

    updateUI();
    statusBar()->showMessage(QString("Initialized %1x%2 sparse matrix.").arg(rows).arg(cols));
}

void MainWindow::clearMatrix()
{
    if (!m_matrix) return;
    int r = m_matrix->getRows();
    int c = m_matrix->getCols();
    createMatrix(r, c); // Reinitialization resets all cells to 0
    statusBar()->showMessage("Matrix successfully cleared.");
}

void MainWindow::updateUI()
{
    if (!m_matrix) return;

    drawGrid();

    int count = m_matrix->getOnesCount();
    m_onesCountLabel->setText(QString("Ones Count: %1").arg(count));

    drawTree();
}

void MainWindow::drawGrid()
{
    m_gridWidget->update();
}

void MainWindow::drawTree()
{
    if (!m_matrix) return;

    // Retrieve fresh, non-dangling root & NIL pointers
    const RBTree& tree = m_matrix->getTree();
    m_treeView->drawTree(tree.getRoot(), tree.getNIL());
}

bool MainWindow::isCellOne(int row, int col) const
{
    if (!m_matrix) return false;
    
    // Bounds check
    if (row < 0 || row >= m_matrix->getRows() || col < 0 || col >= m_matrix->getCols()) {
        return false;
    }
    return m_matrix->isCellOne(row, col);
}

void MainWindow::onCellClicked(int row, int col)
{
    if (!m_matrix) return;

    // Verify coordinate safety before interaction
    if (row >= 0 && row < m_matrix->getRows() && col >= 0 && col < m_matrix->getCols()) {
        m_matrix->toggle(row, col);
        updateUI();

        int state = m_matrix->isCellOne(row, col) ? 1 : 0;
        statusBar()->showMessage(QString("Toggled cell at (%1, %2) to state %3").arg(row).arg(col).arg(state));
    }
}

void MainWindow::onCreateClicked()
{
    createMatrix(m_rowsSpinBox->value(), m_colsSpinBox->value());
}

void MainWindow::onClearClicked()
{
    clearMatrix();
}
