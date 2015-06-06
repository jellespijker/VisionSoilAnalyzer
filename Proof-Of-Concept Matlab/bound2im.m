function [ image ] = bound2im( b, M, N )
% IMAGE = BOUND2IM(b) converts b, a np-by-2 array containing the integer
% coordinates of a boundary, into a binary image with 1s in the locations of
% the cooordinates in b an 0's elsewhere.
% Source: Gonzalez. Digital Image Processing Using MATLAB. McGraw-Hill Education (India) Pvt Limited, 2013.
% Some alterations by Jelle Spijker

% Check input.
if size(b, 2) ~= 2
    error('The boundary must be of size np-by-2');
end

% Make sure the coordinates are integers.
if isinteger(b) ~= 1
    error('Matrix should consist of Integers');
elseif nargin == 1
    Mmin = min(b(:,1)) - 1;
    Nmin = min(b(:,2)) - 1;
    H = max(b(:,1)) - Mmin + 2;
    W = max(b(:,2)) - Nmin + 2;
    M = H + Mmin;
    N = W + Nmin;
end

% Make sure there aren't any 0 indexes
if size(b(b == 0)) > [0 0]
    b = b + 1;
end

% Create the image.
image = zeros(M, N);
linearIndex = sub2ind([M N], b(:,1), b(:, 2));
image(linearIndex) = 1;

end

