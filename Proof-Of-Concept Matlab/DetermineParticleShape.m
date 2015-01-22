clear;
load('matlab2.mat');
[~, n] = size(DB);
warning('off', 'images:imshow:magnificationMustBeFitForDockedFigure')
for i = 1:n
    subplot(3,3,1);
    imshow(DB(i).Ic);
    for j = 1:8
        subplot(3,3,j+1);
        imshow(bound2im(uint16(ifrdescp(DB(i).z,2*j)))); 
    end
    DB(i).Sphericity = input('Enter Sphericity: ');
end
warning('on', 'images:imshow:magnificationMustBeFitForDockedFigure')
%save('matlab.mat', DB);