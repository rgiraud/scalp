This code is free to use for any non-commercial purposes.
It contains an implementation of the SCALP superpixel method proposed in:
[1] - Rémi Giraud, Vinh-Thong Ta and Nicolas Papadakis
      Robust Superpixels using Color and Contour Features along Linear Path
      HAL preprint <hal-01510063>, 2017

Note that the core of the implementation is based on the provided code associated to the following paper:
[2] - Zhengqin Li, Jiansheng Chen
      Superpixel Segmentation using Linear Spectral Clustering
      International Conference on Computer Vision and Pattern Recognition (CVPR), 2015

If you use this code, please cite both [1] and [2].


A contour prior map can be added to our method and provided as an image (255 for highest contour intensity).
The contour detection used method in [1] is available with guidelines at 
https://github.com/pdollar/edges
Other contour detection methods can be found here
https://www2.eecs.berkeley.edu/Research/Projects/CS/vision/grouping/resources.html

(C) Rémi Giraud, 2017
rgiraud@labri.fr, www.labri.fr/~rgiraud
University of Bordeaux, LaBRI, IMB


%%%%%%% MATLAB - C++/MEX %%%%%%%%%

run main.m



%%%%%%% C++ %%%%%%%%%

////// COMPILATION /////

- Need CImg library :  http://cimg.eu/

- To compile : make


////// EXECUTION //////

/SCALP -i img_name [-k superpixel_nbr] [-m compactness]  [-outm output_map_name] [-outb output_border_name]  [-c contour]

Example with contour map:  (make test)
./SCALP -i test_img.jpg -k 300 -m 0.075 -outm test_img_map.png -outb test_img_border.png

On an image list:  (make test_list)
./scripts/test_list.sh ./data/list_file.txt ./data/ 450 0.075



