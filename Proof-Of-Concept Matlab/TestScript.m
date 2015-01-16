%% Init Beaglebone connection
tic;
clear;

bbb = beaglebone_black('192.168.178.20', 'root', 'open');
system(bbb, './pwm_vision_matlab');

formatSpec = 'Setting up de Beaglebone %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);
%% Init the CAM
tic;

cam = videoinput('winvideo', 1, 'MJPG_2048x1536');
cam.FramesPerTrigger = 20;
border = 0;
cam.ROIPosition = [ border border 2048-border 1536-border ];

src = getselectedsource(cam);
src.BacklightCompensation = 'off';
src.Sharpness = 5;
src.Contrast = 40;

formatSpec = 'Setting up de cam %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);
%% Get Images
tic;

system(bbb, 'echo 1 > /sys/class/gpio/gpio51/value && echo 1 > /sys/class/gpio/gpio50/value && echo 1 > /sys/class/gpio/gpio60/value');
start(cam)
M = getdata(cam);
I = M(:,:,:,20);
system(bbb, 'echo 0 > /sys/class/gpio/gpio51/value && echo 0 > /sys/class/gpio/gpio50/value && echo 0 > /sys/class/gpio/gpio60/value');

imshow(I)

formatSpec = 'Getting a image %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);
%% Convert RGB to Intensity
tic;

hcsc = vision.ColorSpaceConverter('Conversion', 'RGB to intensity');
Int = step(hcsc, I);

imshow(Int)

formatSpec = 'Convert RGB to Intensity %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);
%% Convert to BW
tic;

BW = GetLabeledImage(I);

imshow(BW)

formatSpec = 'Convert to BW %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);

%% Remove border blobs
tic;

BWl = RemoveBorderBlobs(BW);

imshow(BWl)

formatSpec = 'Remove border blobs %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);
%% Calulate Area
tic;

area = PSD(BWl);

figure; hist(area);

formatSpec = 'Calulate Area %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);
%% Get color matrix of foreground
tic;

Ic = RemoveBackgroundVectorImage(BW, I);
imshow(Ic);

formatSpec = 'Get Color Matrix %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);

%%  Calc CIE La*b* 
tic;

dis = CIELabSoilColorDistribution(BW, I);

figure; scatterhist(dis(:,1),dis(:,2));

formatSpec = 'Calc CIE La*b* alternative 2 %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);

%% Calc RI
tic;

RI = RISoilColorDistribution(BW, I);

figure; hist(RI);

formatSpec = 'Calc RI %d [sec].';
str = sprintf(formatSpec,toc);
disp(str);

%% Left behind particles

Inot = RemoveBackgroundVectorImage(~BW, I);
imshow(Inot);
