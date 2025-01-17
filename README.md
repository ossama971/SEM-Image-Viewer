# **SEM Image Viewer**  
### **A Desktop Application for Scanning Electron Microscope Image Viewing and Editing**  

---

## **Table of Contents**  
1. [Introduction](#introduction)  
2. [Features](#features)  
3. [Key Features](#key-features)  
4. [Technical Specifications](#technical-specifications)  
5. [Modes of Operation](#modes-of-operation)  
   - [Main View](#main-view)  
   - [Grid View](#grid-view)  
   - [Diff View](#diff-view)  
6. [Getting Started](#getting-started)  
7. [Overview Screenshots](#overview-screenshots)  
8. [High-Level Architecture](#high-level-architecture)  
9. [Acknowledgments](#acknowledgments)  
10. [Contributing](#contributing)  
11. [License](#license)  

---

## **Introduction**  
The SEM Image Viewer is a **desktop application** developed to enhance workflows in **digital chip manufacturing** by enabling **engineers** and **scientists** to view, edit, and analyze images captured by **Scanning Electron Microscopes (SEMs)**. The tool combines robust image processing capabilities with a user-friendly interface, supporting both individual and batch operations for high efficiency.  

This project was developed as part of the **Siemens Software Academy Graduation Program**.  

---

## **Features**  
- **Three Operational Modes**:
  - **Main View**: Focus on a single SEM image for in-depth analysis.  
  - **Grid View**: Compare multiple images simultaneously in a grid format.  
  - **Diff View**: Compare two images side by side with automated difference highlighting.  

- **Dynamic Metadata Bar**:
  - Displays **image dimensions**, **zoom percentage**, and **mouse hover position**.  

- **Real-time Processing**:
  - Multi-threaded architecture for handling large datasets without lag.

- **Logging System**:
  - A comprehensive Logging System with undo/redo options and quick links.

- **UI/UX Considerations**:
  - The tools offer both light and dark mode with shortcuts and multiple access buttons.  

---

## **Key Features**  

### **1. Flexible Image Viewing**  
The application offers multiple modes for viewing SEM images, allowing users to toggle between **detailed single-image views**, **side-by-side comparisons**, and **grid layouts**. This flexibility ensures the application can meet diverse needs, from focused analysis to batch comparisons.  

### **2. Powerful Filters**  
Built-in image processing tools enable users to apply filters such as:  
- **Contrast enhancement**  
- **Noise reduction**  
- **Sharpening**  
These tools help improve image clarity and highlight key features of SEM scans.  

### **3. Different Overlays**  
The viewer supports customizable **image overlays** for visualizing image data using **heat maps** to analyze patterns and **intensity plots** for pixel intensity distribution. These overlays are invaluable for **quality control** and **feature tracking** in manufacturing workflows.  

### **4. Batching Operations**  
Perform operations on multiple images simultaneously, such as applying filters, exporting images, or comparing results. This significantly reduces manual effort in processing large datasets utilizing multithreaded capabilities.  

### **5. Session Management**  
The application allows users to **save their progress** and resume sessions at any time. This includes saving the current view, filters, and annotations, ensuring a seamless workflow.  

### **6. Multi-Format Export**  
Export processed images in multiple formats, such as **JPG**, **PNG**, and **BMP**, with support for custom resolutions and compression settings. This feature is tailored for diverse industry standards.  

### **7. Logging**  
Comprehensive logging of all actions ensures transparency and provides a record of the processing pipeline. Logs can be exported for audit purposes or workflow optimization.  

### **8. Undo/Redo Functionality**  
Every action performed in the application can be reversed or re-applied using an intuitive **Undo/Redo** system, ensuring user confidence and preventing accidental errors.  

### **9. Thread Pool for Multi-threaded Processing**  
The application utilizes a **Thread Pool** to manage multiple concurrent image processing tasks, ensuring that large datasets or computationally intensive tasks do not block the user interface. This results in smoother performance and responsiveness even under heavy workloads.  

### **10. Image Caching**  
To optimize image viewing and processing, the SEM Image Viewer employs an **Image Cacher**. Frequently accessed images are stored in memory, significantly reducing loading times when switching between images or re-applying filters.  

---

## **Technical Specifications**  
- **Framework**: Qt (GUI Development)  
- **Programming Language**: C++  
- **Image Processing Library**: OpenCV  
- **Build System**: CMake  
- **Target Platform**: Windows/Linux  

---

## **Modes of Operation**  

### **Main View**  
- Focuses on a single SEM image.  
- Provides tools for zooming, panning, and applying filters.  
- Displays metadata dynamically (e.g., dimensions, zoom, and hover coordinates).  

### **Grid View**  
- Displays multiple images in a grid layout.  
- Synchronizes zooming across images for batch comparisons.  
- Ideal for analyzing variations between SEM scans.  

### **Diff View**  
- Places two images side by side.  
- Highlights differences using customizable comparison algorithms.  
- Supports overlay features for more detailed insights.  

---

## **Getting Started**  

### **Prerequisites**  
- Install the following tools and libraries:  
  - Qt6 framework.  
  - OpenCV (latest version).  
  - CMake (minimum version 3.22).  
- A C++17-compliant compiler (GCC/Clang/MSVC).  

### **Installation**  
1. Clone the repository:  
 ```bash
 git clone <repository_url>
 ```

2. Build the project using CMake:
  ```bash
  mkdir build && cd build  
  cmake ..  
  make  
  ```

3. Run the application:
  ```bash
  ./SEMImageViewer
  ```

---

## **Overview Screenshots**  

### **Main View**  
*(Insert Main View screenshot here)*  

### **Grid View**  
*(Insert Grid View screenshot here)*  

### **Diff View**  
*(Insert Diff View screenshot here)*  

---

## **High-Level Architecture**  
*(Insert High-Level Architecture diagram here)*  

---

## **Acknowledgments**  
This project was developed as part of the **Siemens Software Academy Graduation Program** by our team. It reflects our dedication to solving real-world challenges in the field of **digital chip manufacturing**.  

---

## **Contributing**  
We welcome contributions from the community.

## **License**  
This project is licensed under the MIT License. See the `LICENSE` file for details.  

---
