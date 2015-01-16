function [ L n ] = GetLabeledImage( I )
%GETLABELEDIMAGE ( I) Convert the color image to a black and white logical image
%
%   Copyright (c) 2014 Jelle Spijker
% 
%  Permission is hereby granted, free of charge, to any person
%  obtaining a copy of this software and associated documentation
%  files (the "Software"), to deal in the Software without
%  restriction, including without limitation the rights to use,
%  copy, modify, merge, publish, distribute, sublicense, and/or sell
%  copies of the Software, and to permit persons to whom the
%  Software is furnished to do so, subject to the following
%  conditions:
% 
%  The above copyright notice and this permission notice shall be
%  included in all copies or substantial portions of the Software.
% 
%  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
%  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
%  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
%  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
%  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
%  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
%  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
%  OTHER DEALINGS IN THE SOFTWARE.

hcsc = vision.ColorSpaceConverter('Conversion', 'RGB to intensity');
Int = step(hcsc, I);
[m, n] = size(Int);

L = zeros(m, n);
H = fspecial('gaussian',[5 5],0.6);

% calculate the treshold and convert to BW
Tr = uint8(mean(Int(:)) - (1/2)*std(double(Int(:))));
BW = zeros(size(Int));
BW(Int < Tr) = 1;

% Close the image (remove small holes and openings)
SE = strel('disk', 20,0);
BWROI = imclose(BW, SE);

% Determine Tresshold more closely for each region and build new picture
hblob = vision.BlobAnalysis;
hblob.AreaOutputPort = true;
hblob.BoundingBoxOutputPort = true;
hblob.MaximumCount = 1000;

[Area, Centroid, Blobs] = step(hblob, logical(BWROI));
[noblobs, ~] = size(Blobs);

for i = 1:noblobs
    % Extract ROI + border from source intensity picture an determine new thresshold
    boxROI = [Blobs(i,2) Blobs(i,2)+Blobs(i,4)-1 Blobs(i,1) Blobs(i,1)+Blobs(i,3)-1];
    expansionBorder = 10;
    if boxROI(1) <= expansionBorder
        boxROI(1) = 1;
    else
        boxROI(1) = boxROI(1) - expansionBorder;
    end
    if boxROI(2) > (m - expansionBorder)
        boxROI(2) = m;
    else
        boxROI(2) = boxROI(2) + expansionBorder;
    end
    if boxROI(3) <= expansionBorder
        boxROI(3) = 1;
    else
        boxROI(3) = boxROI(3) - expansionBorder;
    end
    if boxROI(4) >= (n - expansionBorder)
        boxROI(4) = n;
    else
        boxROI(4) = boxROI(4) + expansionBorder;
    end    
    ROI = Int(boxROI(1):boxROI(2),boxROI(3):boxROI(4));
    
    ROI = imfilter(ROI,H,'replicate');
    level = graythresh(ROI) + ((1/3)*std(double(ROI(:))))/255;
    BWROI = im2bw(ROI,level);
    BWROI = ~BWROI;
    
    % Erode the new region of interrest with a disk of 2.5%
    total = sqrt(double(bwarea(BWROI)));
    se = strel('disk',round(total*0.025));
    erodedI = imerode(BWROI,se);
    
    BWROI = imfill(erodedI, 'holes');
    BWROI = RemoveBorderBlobs(BWROI);
    L(boxROI(1):boxROI(2),boxROI(3):boxROI(4)) = BWROI;
end

L = logical(L);

end

