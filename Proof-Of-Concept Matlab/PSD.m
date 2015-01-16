function [ P ] = PSD( L )
%PSD Summary of this function goes here
%   Detailed explanation goes here

%Determine the Area for the blobs containing soil samples 
hblob = vision.BlobAnalysis;
Area = step(hblob, logical(L));

blobs = size(Area);
P = zeros(sum(Area(:,1)),1);
j = 1;
% Loop through the blobs
for i = 1:blobs
    if i == 1
        P(1:Area(i)) = Area(i);
        j = j + Area(i);
    else
        P(j:(j + Area(i))) = Area(i);
        j = j + Area(i);
    end
end

P = Area2Volume(double(P));
end

