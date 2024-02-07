##Content Based Image Retrieval

Inputs:
- Target image (T)
- Image database (B)
- Method for capturing feature (Feature type) (F)
- Distance metrics to compare feature from two images(D(Ft,Fi))
- Desired number of output images N

Process:
- Compute the features Ft on the target image T
- Compute the features Fi on all the images in B
    - Executing many queries with different target images
    - Features are computed and stored in a CSV file and then used for many different target image
- Compute the distance of T from all the images in B using the distance metric D(Ft,Fi)
- Sort the images in B according to their distance from T and return the best N matches

Ways to find similar images:
- Baseline Matching
- Histogram Matching
- Multi-Histogram Matching
- Texture and color
- Deep Network Embeddings
- Compare DNN embeddings and Classic features

Summary:
- The program takes in target filename for T, directory of images as the database B, the feature type, matching method, and the number of images to return.
- The program prints the filename software the top N similar images.
