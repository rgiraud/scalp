% This code is free to use for any non-commercial purposes.
% It contains an implementation of the SCALP superpixel method proposed in:
% [1] - Rémi Giraud, Vinh-Thong Ta and Nicolas Papadakis
%       Robust Superpixels using Color and Contour Features along Linear Path
%       Computer Vision and Image Understanding (CVIU), 2018
%
% Note that the core of the implementation is based on the provided code associated to the following paper:
% [2] - Zhengqin Li, Jiansheng Chen
%       Superpixel Segmentation using Linear Spectral Clustering
%       International Conference on Computer Vision and Pattern Recognition (CVPR), 2015
%
% If you use this code, please cite both [1] and [2].
%
% A contour prior map can be added to our method. 
% The contour detection used method in [1] is available with guidelines at 
% https://github.com/pdollar/edges
% Other contour detection methods can be found here
% https://www2.eecs.berkeley.edu/Research/Projects/CS/vision/grouping/resources.html
%
% (C) Rémi Giraud, 2017
% rgiraud@labri.fr, www.labri.fr/~rgiraud
% University of Bordeaux, LaBRI, IMB

clear all
close all
clc

%Compilation
mex -O CFLAGS="\$CFLAGS -Wall -Wextra -W -std=c99" ./SCALP_mex.cpp -outdir ./


% Image loading
data_path = './data';
img_name = sprintf('%s/test_img.jpg',data_path); 
I = imread(img_name);
[h,w,z] = size(I);

% Parameters
K = 325;    % Superpixel number
m = 0.075;  % Compactness parameter (default 0.075)

% Contour detection
% Plug here your contour detector C (see readme.txt file)
C = double(imread(sprintf('%s/contour_test_img.png',data_path)));
C = C/max(C(:));

% SCALP 
S = SCALP_mex(uint8(I), K, m, single(C));
%S = SCALP_mex(I,K,m);                     %without contour prior

% Plotting results
B = zeros(h,w);
for i=1:max(S(:))
    j = (S == i);
    bb = bwboundaries(j);
    if ~isempty(bb)
        for k=1:length(bb{1})
            B(bb{1}(k,1),bb{1}(k,2)) = 1;
        end
    end
end

figure,
imagesc(double(I)/255.*repmat(~B,[1 1 3])); 


%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%To use display and evaluation superpixel functions
%%download the toolbox at remigiraud.fr/research/#superpixels
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

gt = imread('data/gt_test_img.png');

if (exist('utils_sp'))
    main_fct_sp(I,gt,S);
end


