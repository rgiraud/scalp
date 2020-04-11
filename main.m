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


