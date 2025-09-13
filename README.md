# Steganography Using LSB (Least Significant Bit)

## 📂 Project Overview
This project demonstrates **steganography** by hiding and extracting secret information within image files using the **Least Significant Bit (LSB)** technique. Implemented in **C programming language**, it covers encoding and decoding methods that allow users to securely conceal messages inside bitmap images without noticeable changes to the image quality.

The project is designed as a practical implementation for learning file handling, bit manipulation, and data security techniques.


## ⚙️ Features
✔ Encode a secret message inside an image using LSB technique  
✔ Decode the hidden message from the image  
✔ Maintain image quality without noticeable distortion  
✔ File handling and bitwise operations implemented in C

## 🛠 Technologies Used
- Programming Language: **C**
- Image Processing using bitmap files
- Bit manipulation for LSB steganography
- File handling for reading and writing data


## 📂 Files Description
- **encode.c / encode.h**: Functions to embed messages into images.
- **decode.c / decode.h**: Functions to extract hidden messages from images.
- **file.c**: File reading and writing utilities.
- **types.h**: Definitions for data types used across the project.
- **test_main.c**: Main program to test encoding and decoding functionalities.
- **beautiful.bmp / stego.bmp / output.bmp**: Input and output images.
- **secret.txt**: File containing the message to encode.
- **output.txt**: File containing the extracted message.

## 📖 Future Improvements
- Add encryption for secure message embedding  
- Support multiple image formats like PNG or JPEG  
- Implement GUI for ease of use  
- Enhance error handling and validation
 

