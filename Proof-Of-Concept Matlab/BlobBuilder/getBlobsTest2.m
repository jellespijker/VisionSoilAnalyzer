clear;
addpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer';

cam = videoinput('winvideo', 1, 'MJPG_2048x1536');
cam.FramesPerTrigger = 1;
cam.ROIPosition = [ 0 0 2048 1536 ];
src = getselectedsource(cam);
src.BacklightCompensation = 'off';
src.Sharpness = 1;
src.Contrast = 50;
src.Brightness = -30;

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
hcsc = vision.ColorSpaceConverter('Conversion', 'RGB to HSV');

Int = step(hcsc, double(I));
%L = normc(Int(:,:,1));
H = Int(5:end-5,5:end-5,1);
S = Int(5:end-5,5:end-5,2);
V = Int(5:end-5,5:end-5,3);

imagesc(V)
    
% rmpath 'D:/OneDrive/Opleiding/HTS HAN/Minor/MatlabSoilAnalyzer';