clear; clc;
I = zeros(640,640);

n = 80;
loc = randi([40 600],n, 2);
for i = 1:n
   Z = zeros(80,80);
   XY = 0;
   K = 0;
   x = 0;
   y = 0;
   sides = 0;
   sides = randi([3 5],1); 
   x = randi([0 80], 1, sides);
   y = randi([0 80], 1, sides);
   K = convhull(x,y, 'simplify', true);
   c = numel(K) - 1;
   count = 0;
   for j = 1:2:(c*2)
      count = count + 1;
      XY(j) = (x(count)-40)+loc(i,1);
      XY(j+1) = (y(count)-40)+loc(i,2);
   end
   
   I = insertShape(I, 'FilledPolygon', XY);
   
end

% Convert RGB image to chosen color space
I = rgb2hsv(I);
channel1Min = 0.180;
channel1Max = 0.152;
channel2Min = 0.000;
channel2Max = 1.000;
channel3Min = 0.000;
channel3Max = 1.000;
I = ( (I(:,:,1) >= channel1Min) | (I(:,:,1) <= channel1Max) ) & ...
    (I(:,:,2) >= channel2Min ) & (I(:,:,2) <= channel2Max) & ...
    (I(:,:,3) >= channel3Min ) & (I(:,:,3) <= channel3Max);
I = ~I;

hopening = vision.MorphologicalOpen;
hopening.Neighborhood = strel('disk', 6);
I = step(hopening, uint8(I));

H = vision.BlobAnalysis;
[AREA, CENTROID, BBOX] = step(H, logical(I));
I = I * 255;
imshow(I);
