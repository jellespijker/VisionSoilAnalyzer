% Soil samples GetSamples
cam = videoinput('winvideo', 1, 'MJPG_2048x1536');
cam.FramesPerTrigger = 1;
cam.ROIPosition = [ 0 0 2048 1536 ];
src = getselectedsource(cam);
src.BacklightCompensation = 'off';
src.Sharpness = 2;
src.Contrast = 40;
%bbb = beaglebone_black('192.168.178.20', 'root', 'open');
%system(bbb, './pwm_vision_matlab');
addpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer'

%system(bbb, 'echo 1 > /sys/class/gpio/gpio51/value && echo 1 > /sys/class/gpio/gpio50/value && echo 1 > /sys/class/gpio/gpio60/value');

DB = struct;
n = 1;

for i = 1:1
    preview(cam);
    disp('Focus sample and press any key when ready');
    pause;
    closepreview;
    
    start(cam)
    I = getdata(cam);
    I = I(5:end-5,5:end-5,:);
    L = logical(GetLabeledImage(I));
    
    hcsc = vision.ColorSpaceConverter('Conversion', 'RGB to intensity');
    Int = step(hcsc, I);
    
    hblob = vision.BlobAnalysis;
    hblob.MaximumCount = 1000;
    [~, ~, BBox] = step(hblob, L);
    [noBlob, ~] = size(BBox);
    
    for j = 1:noBlob
        DB(n).Ic = I(BBox(j,2):BBox(j,2)+BBox(j,4),BBox(j,1):BBox(j,1)+BBox(j,3),:);
        DB(n).I = logical(L(BBox(j,2):BBox(j,2)+BBox(j,4),BBox(j,1):BBox(j,1)+BBox(j,3)));
        DB(n).z = frdescp(DB(n).I);
        DB(n).Sphericity = -1;
        DB(n).Angularity = -1;
        n = n + 1;
    end   
   
end
% system(bbb, 'echo 0 > /sys/class/gpio/gpio51/value && echo 0 > /sys/class/gpio/gpio50/value && echo 0 > /sys/class/gpio/gpio60/value');

[~, noS] = size(DB);
n = 1;

for i = 1:noS
    [xs ys] = (size(DB(i).I));
    if xs <= 25 && ys <= 25
        elRem(n) = i;
        n = n + 1;
    end
end

DB(elRem) = [];

rmpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer'

