function [ RI ] = RISoilColorDistribution( L, I )
%UNTITLED Summary of this function goes here

% Remove the background from the color image
[m n ~] = size(I);
BW = logical(L);

Ic(m, n, 3)=0;
Ic(:,:,1) = I(:,:,1).*uint8(BW);
Ic(:,:,2) = I(:,:,2).*uint8(BW);
Ic(:,:,3) = I(:,:,3).*uint8(BW);
Ic = uint8(Ic);

%Convert the RGB matrix to CIE La*b* colorspace
hcscLAB = vision.ColorSpaceConverter;
hcscLAB.Conversion = 'sRGB to L*a*b*';
LAB = step(hcscLAB, double(Ic));

%Extract seperate values
Lu = LAB(:,:,1);
a = LAB(:,:,2);
b = LAB(:,:,3);

RI(m, n) = 0;
for i = 1:m
     for j = 1:n
         RI(i,j) = (((Lu(i,j)*(a(i,j)^2 + b(i,j)^2)^(1/2))*10^(8.2)))/((b(i,j)*L(i,j)^6));
     end
end

RI = RI(L ~= 0);
end

