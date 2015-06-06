function [ rgbmat, actualsize, meanlab, meanri ] = GenBlob(creationsize, basicshape, roughness, colorrange )
%Create a blob with certain characteristics
%creationsize as a 2 vector
%basicshape 'Circle', 'Rectangle' or Pentangle
%roughness 0.1...0.9 amount of distortion
%colorrange 2x3 RGB matrix

roughness = 1 - roughness;
rmat = uint8((colorrange(2,1) - colorrange(1,1)).*rand(creationsize(1), creationsize(2)) + colorrange(1,1));
gmat = uint8((colorrange(2,2) - colorrange(1,2)).*rand(creationsize(1), creationsize(2)) + colorrange(1,2));
bmat = uint8((colorrange(2,3) - colorrange(1,3)).*rand(creationsize(1), creationsize(2)) + colorrange(1,3));
rgbmat = uint8(zeros(creationsize(1), creationsize(2), 3));
rgbmat(:,:,1) = rmat;
rgbmat(:,:,2) = gmat;
rgbmat(:,:,3) = bmat;


maskcolor = uint8([1]);

m = uint8(zeros(creationsize(1), creationsize(2),1));
switch basicshape
    case 'Circle'
        shapeInserter = vision.ShapeInserter('Shape', 'Circles', 'BorderColor', 'Custom', 'CustomBorderColor', maskcolor, 'Fill', true, 'FillColor', 'Custom', 'CustomFillColor', maskcolor, 'Opacity', 1);
        circle = int32([creationsize(1) / 2, creationsize(2)/2, (min(creationsize) / 2.5)]);
        mask = step(shapeInserter, m, circle);
    case 'Rectangle'
        shapeInserter = vision.ShapeInserter('Shape', 'Rectangles', 'BorderColor', 'Custom', 'CustomBorderColor', maskcolor, 'Fill', true, 'FillColor', 'Custom', 'CustomFillColor', maskcolor, 'Opacity', 1);
        rect = int32([creationsize(2) / 10, creationsize(1)/10, creationsize(2) - (2 * (creationsize(2)/10)), creationsize(1) - (2 * (creationsize(1)/10)) ]);
        mask = step(shapeInserter, m, rect);       
    case 'Pentangle'

end

J = imnoise(mask, 'salt & pepper', roughness);
mask = mask.*J;
hclosing = vision.MorphologicalClose;
strelsize = double(round(min(creationsize)/15));
hclosing.Neighborhood = strel('disk', strelsize);
mask = step(hclosing, mask);
mask(mask == 1) = 0;
mask(mask > 1) = 1;
mask= imfill(mask,'holes');

rgbmat(:,:,1) = rgbmat(:,:,1).*uint8(mask);
rgbmat(:,:,2) = rgbmat(:,:,2).*uint8(mask);
rgbmat(:,:,3) = rgbmat(:,:,3).*uint8(mask);
end

