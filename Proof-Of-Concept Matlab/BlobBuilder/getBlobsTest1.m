clear;
addpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer';

cam = videoinput('winvideo', 1, 'MJPG_2048x1536');
cam.FramesPerTrigger = 1;
cam.ROIPosition = [ 0 0 2048 1536 ];
src = getselectedsource(cam);
src.BacklightCompensation = 'off';
src.Sharpness = 2;
src.Contrast = 40;
bbb = beaglebone_black('192.168.178.20', 'root', 'open');
system(bbb, './pwm_vision_matlab');
addpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer'

system(bbb, 'echo 1 > /sys/class/gpio/gpio51/value && echo 1 > /sys/class/gpio/gpio50/value && echo 1 > /sys/class/gpio/gpio60/value');
preview(cam);
disp('Focus sample and press any key when ready');
pause;
closepreview;
    
start(cam)
I = getdata(cam);
%I = decorrstretch(I);

% Convert the color image to a black and white logical image
H = vision.Autothresholder;
hcsc = vision.ColorSpaceConverter('Conversion', 'RGB to intensity');

Int = step(hcsc, I);
%J = histeq(Int,256);
J = adapthisteq(Int);
BW = step(H, J);
BW = imfill(~BW,'holes');

% Remove the borderblobs
[r, c] = size(BW);
BW(1, :) = 1;
BW(:, 1) = 1;
BW(r, :) = 1;
BW(:, c) = 1;
L = bwlabel(BW,8);
L(L == L(1,1)) = 0;
L(L > 0) = 1;

% Remove the background from the color image
[m, n, ~] = size(I);
BW = logical(L);

Ic = zeros(m, n, 3);
Ic(:,:,1) = I(:,:,1).*uint8(BW);
Ic(:,:,2) = I(:,:,2).*uint8(BW);
Ic(:,:,3) = I(:,:,3).*uint8(BW);
Ic = uint8(Ic);

hblob = vision.BlobAnalysis;
hblob.MaximumCount = 1000;
[~, ~, BBox] = step(hblob, BW);
[noBlob, ~] = size(BBox);
DB = struct;
    
%     for j = 1:noBlob
%         DB(j).Ic = I(BBox(j,2):BBox(j,2)+BBox(j,4),BBox(j,1):BBox(j,1)+BBox(j,3),:);
%         DB(j).I = BW(BBox(j,2):BBox(j,2)+BBox(j,4),BBox(j,1):BBox(j,1)+BBox(j,3));
%         DB(j).z = frdescp(DB(j).I);
%         DB(j).Sphericity = -1;
%         DB(j).Angularity = -1;
%     end
    
rmpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer';