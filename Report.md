# Laporan Analisis Segmentasi Garis

## 1. Analisis Ukuran Jendela (Window) yang Optimal

Ukuran jendela pemindaian (`local_window`) merupakan penentu utama efisiensi dan sensitivitas algoritma deteksi. Diputuskan untuk menggunakan ukuran jendela **3x3 piksel** berdasarkan pertimbangan berikut:

* **Rasio Informasi-Kompleksitas**: Jendela 3x3 adalah konfigurasi paling minimal yang memungkinkan piksel pusat (P) dianalisis bersama semua 8 tetangga terdekatnya. Ukuran ini memberikan data konektivitas dan orientasi lokal yang cukup tanpa menambah beban komputasi secara signifikan.
* **Fokus Lokal**: Jendela yang lebih besar (misalnya 5x5) meningkatkan risiko inklusi *noise* atau fitur yang tidak relevan, yang dapat mengganggu akurasi deteksi segmen garis tunggal. Ukuran 3x3 menjaga fokus deteksi tetap *lokal*.

## 2. Evolusi Metode Deteksi: Dari Pola Kaku ke Heuristik Adaptif

Tujuan utama proyek adalah membangun detektor yang andal. Proses pengembangan ini melalui dua tahap, dimulai dari *pattern matching* yang gagal, hingga implementasi heuristik yang fleksibel.

### Tahap 1: Keterbatasan Pencocokan Pola Ideal (*Pattern Matching*)

Pendekatan awal adalah mengandalkan kecocokan biner yang ketat (100% *match*) antara *local window* 3x3 dan pola ideal (Horizontal, Vertikal, Diagonal) yang didefinisikan secara eksplisit.

* **Masalah Utama**: **Ketidaksempurnaan Rasterisasi**. Dalam praktiknya, terutama pada garis diagonal dan garis yang dihasilkan oleh `QPainter` (termasuk *anti-aliasing*), tidak ada *local window* yang persis sama dengan pola ideal yang kaku. Misalnya, garis diagonal di grid piksel selalu berupa "tangga," dan penebalan garis (lebar 5 piksel) membuat pola tengah menjadi `1 1 1 / 1 1 1 / 1 1 1`, yang tidak sesuai dengan pola yang diinginkan seperti `0 1 0 / 0 1 0 / 0 1 0`.
* **Hasil Pengujian Awal**: Metode ini menghasilkan *output* yang konsisten **"Ditemukan 0 kandidat segmen"** di log, mengonfirmasi kegagalan *pattern matching* kaku.

### Tahap 2: Implementasi Algoritma Heuristik Berbasis Konektivitas (Solusi Optimal)

Untuk mengatasi masalah rasterisasi dan presisi, metode dialihkan dari mencari *pola* sempurna menjadi menganalisis **struktur dasar konektivitas** sebuah piksel.

**Logika Inti Algoritma Heuristik**:
Metode ini berfokus pada penghitungan jumlah tetangga aktif (berwarna) di sekitar piksel pusat (P) dalam jendela 3x3. Sebuah piksel `P` diklasifikasikan sebagai bagian dari segmen garis jika:

1.  **Piksel Pusat Aktif**: Piksel `P` adalah bagian dari garis yang digambar (bukan latar belakang).
2.  **Jumlah Tetangga Aktif (N) $\leq 2$**: Piksel yang dikelilingi oleh terlalu banyak tetangga aktif (> 2) diabaikan, karena biasanya mengindikasikan **persimpangan** atau **garis tebal non-segmen**.

Jumlah Tetangga Aktif (N),Implikasi Logis,Klasifikasi
N = 1,Piksel hanya terhubung ke satu piksel lain.,Mengidentifikasi Ujung Segmen Garis (Endpoint).
N = 2,Piksel terhubung ke piksel sebelumnya dan berikutnya.,Mengidentifikasi Bagian Tengah Segmen Garis (Interior Point).

Pendekatan heuristik ini terbukti jauh lebih **robust** karena ia tidak peduli dengan orientasi garis (horizontal, vertikal, diagonal) atau ketidaksempurnaan rasterisasi, melainkan hanya pada sifat topologis dari sebuah segmen garis tunggal.