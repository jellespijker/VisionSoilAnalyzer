function [ Dis, LAB] = CIELabSoilColorDistribution( L, I )
%CIELabSoilColorDistribution Summary of this function goes here
%   Detailed explanation goes here

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

%Determine the ROI containing soil samples 
hblob = vision.BlobAnalysis;
[Area, ~, BBox] = step(hblob, BW);

%Setup Distrubtion matrix first column is a*, second column is b* and
%third column is the theoretical volume of the blob
[blobs, ~] = size(BBox);
Dis = zeros(blobs,3);

% Loop through the ROI's
for i = 1:blobs
    %Extract ROI from CIE La*b* matrix using the earlier determined ROI 
    LABblob = LAB(BBox(i,2):BBox(i,2)+BBox(i,4),BBox(i,1):BBox(i,1)+BBox(i,3),:);
    
    %Extract seperate values
    Lu = LABblob(:,:,1);
    a = LABblob(:,:,2);
    b = LABblob(:,:,3);
    
    % Values with no luminosity, namely background get the value NaN
    a(Lu == 0) = NaN;
    b(Lu == 0) = NaN;
    
    % Calculate the mean for each blob on the a* and b* axis
    Dis(i,1) = nanmean(a(:));
    Dis(i,2) = nanmean(b(:));
    
    % calculate the theoretical volume of the particle
    % Dis(i,3) = Area2Volume(double(Area(i)));
    Dis(i,3) = Area(i);
end

end

