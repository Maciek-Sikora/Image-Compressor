# Image-Compressor
![image](https://github.com/Maciek-Sikora/Image-Compressor/assets/43787380/9c07201d-088c-4143-b9ae-b81440dcb5fb)
## What does this application do?
This Qt application is designed as an image compression tool. It allows users to select an image file, adjust compression parameters, and then compress the image using Singular Value Decomposition (SVD). The user interface provides feedback on the compression process and allows users to save the compressed image to a file. Overall, the app streamlines the process of compressing images using advanced linear algebra, providing a user-friendly experience for image compression tasks.
## Functionalities
This Qt application allows users to compress images using Singular Value Decomposition (SVD). Initially, a start window is displayed where users can select an image file. Once an image is selected, its dimensions, size, and other statistics are displayed. Users can either drag and drop image files onto the main window or use a file dialog to select an image. They can then adjust a slider to choose the number of singular values (k) used in the compression process. Upon adjusting the slider, the application computes the compressed image using RSVD (Randomized Singular Value Decomposition). A progress bar visually indicates the compression progress. Finally, users can save the compressed image to a file.
## Demo
https://github.com/Maciek-Sikora/Image-Compressor/assets/43787380/80ff7327-6a8b-499e-b63c-5757de671252
## SVD
<div >
  <img src="https://github.com/Maciek-Sikora/Image-Compressor/assets/43787380/89061bef-a832-479a-8356-8df8030b102d" width="500">
</div>

Singular Value Decomposition (SVD) is a mathematical method used in linear algebra to decompose a matrix into three components: U, Σ, and V^T. Here, U and V are orthogonal matrices, and Σ is a diagonal matrix containing singular values. These singular values represent the importance of each component in the original matrix.

In the context of image compression, we apply SVD to decompose an image matrix into its color channels: red, green, and blue. Each channel is represented as a matrix, and SVD allows us to approximate these matrices with fewer singular values, thus reducing storage space while maintaining image quality.

By selecting a certain number of singular values (k), we can control the quality of the approximation. Higher values of k result in better quality but require more storage space. The compressed size of the approximation is proportional to the product of k and the dimensions of the image channels.

<div >
  <img src="https://github.com/Maciek-Sikora/Image-Compressor/assets/43787380/8d8708d9-8d7c-4e74-bd55-780e2ac80e11" width="500">
</div>

## RSVD

Randomized Singular Value Decomposition (RSVD) is a method that approximates the traditional Singular Value Decomposition (SVD) with reduced computational complexity. By introducing randomization techniques, RSVD efficiently computes an approximation of the SVD using a smaller subset of the matrix's elements. This process involves random projection, computing a reduced SVD, and reconstructing approximate singular vectors. RSVD offers faster computation times and reduced memory requirements, making it useful for large-scale matrix problems in fields like machine learning and signal processing. While not always as accurate as exact SVD, RSVD's computational benefits make it valuable for many practical applications.

References:

The algorithm was constructed based on this paper: https://research.facebook.com/blog/2014/9/fast-randomized-svd/
