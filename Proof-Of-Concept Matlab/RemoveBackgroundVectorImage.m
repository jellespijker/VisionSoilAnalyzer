function [ Ic ] = RemoveBackgroundVectorImage( BW, I )
%RemoveBackgroundVectorImage  Remove the background from the color image
%   BW is the logical image matrix, I is the three vector matrix

Ic = zeros(size(I));
[~, ~, n] = size(I);
for i = 1:n
    Ic(:,:,i) = I(:,:,i).*uint8(BW);
end
Ic = uint8(Ic);

end

