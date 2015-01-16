
%load('matlab.mat');

[~, n] = size(DB);
I = complex(zeros(n, 6));
%cat = zeros(n,3);
cat = zeros(n,1);
for i = 1:n
   I(i,:) = DB(i).z(1:6);
   %cat(i, DB(i).Sphericity) = 1;
   cat(i) = DB(i).Sphericity;
end

Input = zeros(n,12);
Input(:,1:6) = real(I);
Input(:,7:12) = imag(I);