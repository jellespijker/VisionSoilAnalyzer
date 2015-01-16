function [ L ] = RemoveBorderBlobs( BW )
%RemoveBorderBlobs Remove blobs on the border of an image
%   Detailed explanation goes here

[r, c] = size(BW);
BW(1, :) = 1;
BW(:, 1) = 1;
BW(r, :) = 1;
BW(:, c) = 1;
L = bwlabel(BW,8);
L(L == L(1,1)) = 0;
L(L > 0) = 1;

L = logical(L);

end

