#include "drawingcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>

// Gunakan using std::array untuk tipe data Array3x3
using Array3x3 = std::array<std::array<bool, 3>, 3>;

DrawingCanvas::DrawingCanvas(QWidget *parent) : QWidget(parent) {
    qDebug() << "--- MENGGUNAKAN KODE VERSI FINAL ---";

    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    setStyleSheet("background-color: white; border: 1px solid gray;");

    // BARU: Inisialisasi pola-pola ideal untuk deteksi segmen garis.
    // Ini adalah 'detector' yang akan kita gunakan.
    m_idealPatterns.append(Array3x3{{{0,0,0}, {1,1,1}, {0,0,0}}}); // Horizontal
    m_idealPatterns.append(Array3x3{{{0,1,0}, {0,1,0}, {0,1,0}}}); // Vertikal
    m_idealPatterns.append(Array3x3{{{1,0,0}, {0,1,0}, {0,0,1}}}); // Diagonal \
    m_idealPatterns.append(Array3x3{{{0,0,1}, {0,1,0}, {1,0,0}}}); // Diagonal /
}

void DrawingCanvas::clearPoints(){
    m_points.clear();
    m_candidatePoints.clear(); // BARU: Pastikan kandidat juga dibersihkan.
    isPaintLinesClicked = false;
    update();
}

void DrawingCanvas::paintLines(){
    isPaintLinesClicked = true;
    update();
}

// BARU: Fungsi untuk membandingkan dua window 3x3
bool DrawingCanvas::compareWindows(const Array3x3& w1, const Array3x3& w2) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (w1[i][j] != w2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// UBAH: Nama fungsi diubah menjadi detectAndMarkSegments agar sesuai dengan file header.
void DrawingCanvas::detectAndMarkSegments(){
    m_candidatePoints.clear(); // Kosongkan daftar kandidat sebelum deteksi baru
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();

    qDebug() << "Memulai deteksi segmen...";

    // Iterasi melalui setiap piksel di gambar (dengan batas 1 piksel agar tidak keluar dari array)
    for(int i = 1; i < image.width() - 1; i++){
        for(int j = 1; j < image.height() - 1; j++){
            Array3x3 local_window = {{{0}}}; // Window 3x3 lokal untuk piksel saat ini
            bool is_window_empty = true;

            // Isi local_window dengan data piksel dari gambar
            for(int m = -1; m <= 1; m++){
                for(int n = -1; n <= 1; n++){
                    QColor color = image.pixelColor(i + m, j + n); // Ambil warna piksel

                    // GANTI: Gunakan Thresholding (batas nilai)
                    // Jika salah satu komponen warna (Red, Green, atau Blue)
                    // nilainya di bawah 240 (dari nilai maksimum 255),
                    // maka anggap piksel tersebut BUKAN PUTIH dan bagian dari garis.
                    if (color.red() < 240 || color.green() < 240 || color.blue() < 240) {
                        local_window[n + 1][m + 1] = true;
                        is_window_empty = false;
                    }
                }
            }

            // BARU: Jika window tidak kosong, bandingkan dengan semua pola ideal
            if (!is_window_empty) {
                for(const auto& idealPattern : std::as_const(m_idealPatterns)){
                    if(compareWindows(local_window, idealPattern)){
                        // Jika cocok, tambahkan titik tengah window sebagai kandidat
                        m_candidatePoints.append(QPoint(i, j));
                        break; // Hentikan pencarian jika sudah ketemu satu pola yang cocok
                    }
                }
            }
        }
    }
    qDebug() << "Deteksi selesai. Ditemukan" << m_candidatePoints.size() << "kandidat segmen.";
    update(); // Panggil update untuk menggambar ulang dengan kandidat yang ditemukan
}

// drawingcanvas.cpp
// ...

void DrawingCanvas::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Gambar titik biru yang diklik pengguna
    QPen pen(Qt::blue, 5); // PERUBAHAN: Lebar pen 5
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));
    for (const QPoint& point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    // Jika tombol 'Paint Lines' diklik, gambar garis merah
    if(isPaintLinesClicked){
        pen.setColor(Qt::red);
        pen.setWidth(5); // PERUBAHAN: Lebar garis 5 untuk deteksi
        painter.setPen(pen);
        for(int i=0; i < m_points.size() - 1; i+=2){
            painter.drawLine(m_points[i], m_points[i+1]);
        }
    }

    // BARU: Gambar persegi ungu untuk setiap kandidat yang terdeteksi
    // Ini adalah output visual dari algoritma deteksi (Tujuan 3).
    pen.setColor(Qt::magenta); // Di instruksi warnanya purple, magenta sangat mirip.
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush); // Tidak perlu diisi warna
    for (const QPoint& candidate : std::as_const(m_candidatePoints)) {
        // Gambar persegi 5x5 di sekitar titik kandidat
        painter.drawRect(candidate.x() - 2, candidate.y() - 2, 5, 5);
    }
}


void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    m_points.append(event->pos());
    m_candidatePoints.clear(); // Hapus tanda ungu saat pengguna menggambar titik baru
    update();
}
