function [ Dis, LAB] = CIELabSoilColorDistribution( L, I )
%CIELabSoilColorDistribution Summary of this function goes here
%   Detailed explanation goes here

% Remove the background from the color image
[m, n, ~] = size(I);
BW = logical(L);

Ic = zeros(m, n, 3);
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

% Returns the distribution
[totSample, ~] = size(a(Lu ~= 0));
Dis = zeros(totSample, 2);
Dis(:, 1) = a(Lu ~= 0);
Dis(:, 2) = b(Lu ~= 0);
end

