#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

// Definisikan kembali kelas MainWindow
// karena file header Anda memiliki definisi kelas yang benar
// dan file .cpp yang diunggah tampaknya salah/tidak lengkap
// (mengandung #ifndef/#define dari header)

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // 1. Buat instance DrawingCanvas
    m_canvas = new DrawingCanvas(this);

    // 2. Buat instance tombol
    m_clearButton = new QPushButton("Clear (Hapus Gambar)", this);
    m_drawButton = new QPushButton("Paint Lines (Gambar Garis)", this);
    m_detectButton = new QPushButton("Detect & Mark Segments (Deteksi Segmen)", this);

    // 3. Atur layout tombol dalam QHBoxLayout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addWidget(m_drawButton);
    buttonLayout->addWidget(m_detectButton);

    // 4. Atur layout utama (canvas dan tombol) dalam QVBoxLayout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_canvas);
    mainLayout->addLayout(buttonLayout);

    // 5. Buat widget sentral untuk menampung mainLayout
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Opsional: Atur ukuran window utama agar sesuai dengan canvas dan tombol
    this->setFixedSize(m_canvas->minimumSizeHint().width() + 600,
                       m_canvas->minimumSizeHint().height() + 400);

    // 6. Hubungkan sinyal (signals) ke slot (slots)

    // Tombol 'Clear' dihubungkan ke slot clearPoints() di canvas
    connect(m_clearButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::clearPoints);

    // Tombol 'Paint Lines' dihubungkan ke slot paintLines() di canvas
    connect(m_drawButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::paintLines);

    // Tombol 'Detect' dihubungkan ke slot detectAndMarkSegments() di canvas
    connect(m_detectButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::detectAndMarkSegments);
}

MainWindow::~MainWindow()
{
    // Destruktor tidak perlu diubah karena parent akan menghapus child-nya
}
