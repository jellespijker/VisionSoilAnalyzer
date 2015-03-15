function [ XYZ ] = convert_RGB2XYZ( RGB )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
s = size(RGB(:,:,:));
A = [0.412453000000000 0.357580000000000 0.180423000000000;
    0.212671000000000 0.715160000000000 0.072169000000000;
    0.019334000000000 0.119193000000000 0.950227000000000];

XYZ = zeros(s(1),s(2),3);

for row = 1:s(1)
     for col = 1:s(2)
        t = RGB(row,col,:);
        R = (double(t(1))/255);
        G = (double(t(2))/255);
        B = (double(t(3))/255);
        v = [R, G, B];
        T = A*double(v');
        XYZ(row, col, 1)= T(1);
        XYZ(row, col, 2)= T(2);
        XYZ(row, col, 3)= T(3);
     end
end

