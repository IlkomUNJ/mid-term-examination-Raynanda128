[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/ZCO_vauY)
Repository for final lesson before mid term of computer graphic course

# NAMA  : RAYNANDA RAMADHAN PUTRA AURELY
# NIM   : 1313623063
# PRODI : Ilmu Komputer 2023 B

# Laporan Akhir Proyek: Segment Detector Application

Aplikasi ini mengimplementasikan detektor segmen garis (horizontal, vertikal, dan diagonal) menggunakan algoritma *pattern matching* dan heuristik konektivitas pada Qt Framework (C++).

## 1\. Analisis Ukuran Jendela dan Metode Deteksi

### Ukuran Jendela (Window) Optimal

Ukuran jendela pemindaian yang digunakan adalah **3x3 piksel**. Ukuran ini dipilih karena menyediakan informasi kontekstual minimum (piksel pusat dan 8 tetangga) yang cukup untuk menentukan orientasi dan konektivitas lokal, sekaligus menjaga efisiensi komputasi yang tinggi.

### Evolusi Metode Deteksi

Metode deteksi mengalami evolusi signifikan untuk mengatasi tantangan *rasterisasi* garis pada layar.

#### A. Keterbatasan Awal: Pencocokan Pola Kaku

Awalnya, proyek mengandalkan kecocokan biner yang ketat (100% *match*) terhadap pola 3x3 ideal yang didefinisikan secara eksplisit. Namun, pengujian logis menunjukkan **kegagalan total** (output **"Ditemukan 0 kandidat segmen"**), terutama karena:

1.  **Rasterisasi**: Garis yang digambar komputer (terutama diagonal) menggunakan aproksimasi (seperti Algoritma Bresenham) yang tidak cocok dengan pola biner kaku.
2.  **Penebalan Garis**: Garis tebal yang di-*render* (5 piksel) mengisi jendela 3x3 dengan pola yang terlalu padat, jauh berbeda dari pola ideal yang memiliki nilai '0' (putih).

#### B. Solusi Final: Algoritma Heuristik Berbasis Konektivitas

Untuk mengatasi kegagalan *pattern matching*, metode dialihkan ke analisis heuristik yang lebih fleksibel, berfokus pada **konektivitas piksel**.

**Logika Heuristik Inti**: Sebuah piksel pusat **P** diklasifikasikan sebagai bagian dari segmen garis jika setelah di-*thresholding*, jumlah piksel tetangga aktif (**N**) berada dalam rentang $1 \leq N \leq 2$.

  * **N = 1**: Mengidentifikasi **Ujung Segmen** (Endpoint).
  * **N = 2**: Mengidentifikasi **Bagian Tengah Segmen** (Interior Point).
  * **N \> 2**: Diabaikan (dianggap sebagai persimpangan atau noise/garis tebal).

Metode ini terbukti lebih *robust* karena menganalisis struktur garis alih-alih bentuk polanya.

## 2\. Analisis Implementasi dan Debugging

| Komponen | Implementasi Kunci | Status Debugging |
| :--- | :--- | :--- |
| **Gambar Garis** | Lebar `QPen` di `paintEvent` diatur menjadi **5 piksel**. | ✔️ Selesai |
| **Binarisasi** | Digunakan **Thresholding RGB** (`< 240`) untuk mengatasi *anti-aliasing*. | ✔️ Selesai |
| **Pola Ideal** | Pola 3x3 disimpan dalam `QVector<Array3x3>`. | ✔️ Selesai |
| **Koneksi UI** | `mainwindow.cpp` menghubungkan tombol ke slot `DrawingCanvas`. | ✔️ Selesai |


https://gemini.google.com/share/085590935585
