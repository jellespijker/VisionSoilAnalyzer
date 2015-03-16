function [ LAB ] = XYZ2LAB( XYZ )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
s = size(XYZ(:,:,:));

LAB = zeros(s(1),s(2),3);
W =[0.9504    1.0000    1.0889];

for row = 1:s(1)
     for col = 1:s(2)
        t = XYZ(row,col,:);
        X = t(1);
        Y = t(2);
        Z = t(3);
        yy0 = Y / W(2);
        xx0 = X / W(1);
        zz0 = Z / W(3);
        L = 0.0;
        A = 0.0;
        B = 0.0;
        if yy0 > 0.008856
            L = (116 * yy0^(1/3)) - 16;
        else
            L = 903.3 * yy0;
        end
        
        A = 500 * (f(xx0) - f(yy0));
        B = 200 * (f(yy0) - f(zz0));
        
        LAB(row, col, 1)= L;
        LAB(row, col, 2)= A;
        LAB(row, col, 3)= B;
     end
end

end

