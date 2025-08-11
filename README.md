# Student Records Management Project

## 📌 Overview
This project contains three C programs to manage binary student records:

1. **`crear`** – Creates an empty file with a given name and permissions.  
2. **`file_creator`** – Generates a binary file containing predefined student records.  
3. **`combine`** – Merges two student files, sorts them by grade, and produces a CSV with grade distribution statistics.

---

## 📂 Files
- **`crear.c`** – Create an empty file with given octal permissions.  
- **`file_creator.c`** – Writes a predefined list of `struct alumnos` records to a binary file.  
- **`combine.c`** – Reads two student binary files, merges and sorts them, outputs a merged file, and creates `estadisticas.csv`.  
- **`Makefile`** – Automates compilation.

---

## 📜 Usage

### 1️⃣ Create an Empty File
```bash
./crear <filename> <octal_mode>
```
### 2️⃣ Generate Student Data File
```bash
./file_creator <filename>
```
### 3️⃣ Merge and Sort Files
```bash
./combine <course_file1> <course_file2> <output_file>
```
## This will:
- Merge and sort by nota ascending
- Write to merged_students.dat
- Produce estadisticas.csv with category counts and percentages

### 📊 estadisticas.csv Format
```bash
M;<count>;<percentage>%
S;<count>;<percentage>%
N;<count>;<percentage>%
A;<count>;<percentage>%
F;<count>;<percentage>%
```

### 📝Notes
```bash
#Record Format
struct alumnos {
    char nombre[50];
    int nota;
    int convocatoria;
};
```
  - combine supports max 100 students total.
  - Invalid grades are excluded from statistics.
